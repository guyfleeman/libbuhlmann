//
// Created by guyfleeman on 11/28/20.
//

#include <cmath>
#include <fstream>

#include "BuhlmannModel.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;
using namespace buhlmann::model;

Model::Model(buhlmann::model::BuhlmannModelVersion version, DiverParameters &ctx) {
    algorithmVersion = version;
    diverCtx = ctx;
}

void Model::initialize()
{
    if (algorithmVersion == BuhlmannModelVersion::ZHL_16C) {
        for (size_t i = 0; i < ZHL_16C_COMPARTMENTS; i++) {
            compartments[i].setVersion(algorithmVersion);
            compartments[i].setNum(i);
        }
    }
}

void Model::clearModel() {
    //std::cout << "Initializing State" << std::endl;

    if (algorithmVersion == BuhlmannModelVersion::ZHL_16C) {
        for (size_t i = 0; i < ZHL_16C_COMPARTMENTS; i++) {
            compartments[i].setPressureN2(updateLowLevelDiffusion_Depth(0.0f, BUHLMANN_RQ, SURFACE_PCT_N2));
            compartments[i].setPressureHe(updateLowLevelDiffusion_Depth(0.0f, BUHLMANN_RQ, SURFACE_PCT_HE));
            //std::cout << compartments[i] << std::endl;
        }

        //std::cout << std::endl;
    }
}

float Model::getCompositeCeilingAtm() {
    float currentCeiling = 0.0f;
    for (auto & cp : compartments) {
        currentCeiling = max(cp.getCeiling(), currentCeiling);
    }

    return currentCeiling;
}

float Model::getCompositeNoDecompressionTime()
{
    float currentNoDecompressionTime = INFINITY;
    for (auto & cp : compartments) {
        currentNoDecompressionTime = min(cp.getCeiling(), currentNoDecompressionTime);
    }

    return currentNoDecompressionTime;
}

void Model::setGradientFactorSlope(float startStopDepth, float endStopDepth)
{
    if (endStopDepth - startStopDepth == 0.0f) {
        throw "BuhlmannModel.cpp: ERROR - invalid slope parameters";
    }

    gradientFactorSlope =
            (diverCtx.getGradientFactorHi() - diverCtx.getGradientFactorLo())
            / (endStopDepth - startStopDepth);
}

float Model::gradientFactor(float currentStopDepth)
{
    return (gradientFactorSlope * currentStopDepth) + diverCtx.getGradientFactorHi();
}

void Model::runForWorkPlan(WorkPlan &wp) {
	for (auto & wpe : wp.travelEntries) {
		runForWorkPlanEntry(wpe);
	}

	std::cout << "END OF PLANNING MODEL " << std::endl;

	generateDecompressionSchedule(wp);
}

void Model::runForWorkPlanEntry(WorkPlanEntry &wpe) {
    for (auto & cp : compartments) {
        updateCompartmentStatic(wpe.depth, wpe.bottomTime, cp, wpe.getAssignedGas());
        //std::cout << cp << std::endl;
    }
    //std::cout << std::endl;

    //std::cout << "Round Ceiling ATM: " << getCompositeCeilingAtm() << std::endl;
    //std::cout << "Round Ceiling: " << convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm()) << std::endl;
    //std::cout << "Round NDL: " << getCompositeNoDecompressionTime() << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;
}

void Model::runExplicitLogForWorkPlan(planning::WorkPlan &wp)
{
    ofstream tc_abs_file;
    ofstream tc_rel_file;
    ofstream tc_ceil_file;
    ofstream total_ceil_atm_file;
    ofstream total_ceil_ft_file;
    tc_abs_file.open ("tc_abs.txt");
    tc_rel_file.open ("tc_rel.txt");
    tc_ceil_file.open ("tc_ceil.txt");
    total_ceil_atm_file.open ("ceil_atm.txt");
    total_ceil_ft_file.open ("ceil_ft.txt");

    float time = 0.0f;
    float LOG_INTERVAL_S = 15.0f;
    for (WorkPlanEntry &wpe : wp.travelEntries) {
        WorkPlanEntry dwpe(wpe);
        dwpe.bottomTime = dwpe.bottomTime / LOG_INTERVAL_S;

        int numIterations = (int) floor(wpe.bottomTime / LOG_INTERVAL_S);
        for (int i = 0; i < numIterations; i++) {
            time += LOG_INTERVAL_S;

            for (auto & cp : compartments) {
                updateCompartmentStatic(dwpe.depth, dwpe.bottomTime, cp, dwpe.getAssignedGas());
            }

            tc_abs_file << time << " ";
            tc_rel_file << time << " ";
            tc_ceil_file << time << " ";
            for (auto & cp : compartments) {
                float curPressure = dwpe.depth / FT_H2O_PER_ATM + SURFACE_PRESSURE;

                tc_abs_file << max(cp.getCeiling(), 0.0f) << " ";
                tc_rel_file << max(cp.getCeiling() / (curPressure * (SURFACE_PCT_N2 + SURFACE_PCT_HE)), 0.0f) << " ";
                tc_ceil_file << max(convertCompositeCeilingPressureToDepth(cp.getCeiling()), 0.0f) << " ";
            }
            tc_abs_file << std::endl;
            tc_rel_file << std::endl;
            tc_ceil_file << std::endl;

            total_ceil_atm_file << time << " " << getCompositeCeilingAtm() << std::endl;
            total_ceil_ft_file << time << " " << convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm()) << std::endl;
        }
    }

    tc_abs_file.close();
    tc_rel_file.close();
    tc_ceil_file.close();
    total_ceil_atm_file.close();
    total_ceil_ft_file.close();
}

void Model::generateDecompressionSchedule(WorkPlan &wp)
{
    const float SURFACE_THRESHOLD = 0.1f;
    const float STOP_DEPTH_INTERVAL = 10.0f;
    const float LAST_STOP_DEPTH = 10.0f;
    const float STOP_TIME_INTERVAL = 60.0f;

    float currentCeiling = convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm());
    float currentDescritizedCeilingTarget = 0.0f;
    float nextDescritizedCeilingTarget = 0.0f;

    while (currentCeiling >= SURFACE_THRESHOLD) {
        // calculate discritizations
        // divers can't hold depth to fractions of a foot, certainly cant maintain an ascent rate
        // this breaks off static stops for deco
        currentCeiling = convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm());
        currentDescritizedCeilingTarget = floor(currentCeiling / STOP_DEPTH_INTERVAL) * STOP_DEPTH_INTERVAL + STOP_DEPTH_INTERVAL;
        nextDescritizedCeilingTarget = currentDescritizedCeilingTarget - STOP_DEPTH_INTERVAL;

        // handle surfacing clearance
        if (nextDescritizedCeilingTarget < LAST_STOP_DEPTH) {
            nextDescritizedCeilingTarget = LAST_STOP_DEPTH;
        }

        // pick a gas
        auto &bg = const_cast<BreathingGas &>(wp.getOptimizedGasForDepth(currentDescritizedCeilingTarget));
        //std::cout << "chose optimized gas: " << bg << std::endl;
        //std::cout << "targeting " << currentDescritizedCeilingTarget;

        // make plan at target
        WorkPlanEntry decoEntry(currentDescritizedCeilingTarget, STOP_TIME_INTERVAL, std::make_shared<BreathingGas>(bg));
        decoEntry.type = DECO_TRAVEL_ENTRY;

        // deep copy the current state for exploration
        std::array<Compartment, ZHL_16C_COMPARTMENTS> compartmentsScratchSpace;
        for (size_t i = 0; i < compartmentsScratchSpace.size(); i++) {
            compartmentsScratchSpace[i] = Compartment(compartments[i]);
        }

        // run until we hit next target
        int iterations;

        // run until we clear the stop
        // this terminating condition is straight forward and should be self-documenting
        for(iterations = 1; (currentDescritizedCeilingTarget != LAST_STOP_DEPTH && currentCeiling > nextDescritizedCeilingTarget)
        || (currentDescritizedCeilingTarget == LAST_STOP_DEPTH && currentCeiling > SURFACE_THRESHOLD); iterations++) {
            // run plan 60 STOP_TIME_INTERVAL at a time
            float updatedCeilingAtm = 0.0f;
            for (auto & cp : compartmentsScratchSpace) {
                updateCompartmentStatic(decoEntry.depth, decoEntry.bottomTime, cp, decoEntry.getAssignedGas());
                updatedCeilingAtm = max(cp.getCeiling(), updatedCeilingAtm);
            }

            //std::cout << "ran depth " << decoEntry.depth << " for " << (float) iterations * STOP_TIME_INTERVAL << std::endl;
            //std::cout << "new ceiling ATM: " << updatedCeilingAtm << std::endl;
            float updatedCeiling = convertCompositeCeilingPressureToDepth(updatedCeilingAtm);
            //std::cout << "new ceiling: " << updatedCeiling << std::endl;

            currentCeiling = updatedCeiling;
        }

        // update the stop runtime and add deco entry
        decoEntry.bottomTime = (float) (iterations - 1) * STOP_TIME_INTERVAL;
        runForWorkPlanEntry(decoEntry);

        wp.travelEntries.emplace_back(decoEntry);
    }
}

float Model::ambientPressureFromDepth(float d)
{
    return (d / 33.0f) + SURFACE_PRESSURE;
}

float Model::updateLowLevelDiffusion_Depth(float depth, float rq, float internalGasRatio)
{
    // perform alveolar model update
    return updateLowLevelDiffusion(ambientPressureFromDepth(depth), rq, internalGasRatio);
}

float Model::updateLowLevelDiffusion(float ambientPressure, float rq, float internalGasRatio) {
    // perform alveolar model update
	return (ambientPressure - WATER_VAPOR_PRESSURE + ((1 - rq) / rq) * CO2_PRESSURE) * internalGasRatio;
}

void Model::updateCompartmentStatic(float depth, float time, Compartment &cp, const BreathingGas &bg)
{
	float partialAlveolarN2, partialAlveolarHe;

	// get diffusion information for relative depth and expected model loading
    partialAlveolarN2 = updateLowLevelDiffusion_Depth(depth, BUHLMANN_RQ, bg.getFracN2());
    partialAlveolarHe = updateLowLevelDiffusion_Depth(depth, BUHLMANN_RQ, bg.getFracHe());
    //std::cout << "\tLL P. Alv N2: " << partialAlveolarN2 << std::endl;
    //std::cout << "\tLL P. Alv He: " << partialAlveolarHe << std::endl;

    // update compartment pressures
    //std::cout << "\tCPi P. N2: " << cp.getPressureN2() << std::endl;
    //std::cout << "\tCPi P. He: " << cp.getPressureHe() << std::endl;
	cp.setPressureN2(updateHaldaneGas(cp.getPressureN2(), partialAlveolarN2, time,
                                      cp.getConstants().nitrogenHalfTime));
	cp.setPressureHe(updateHaldaneGas(cp.getPressureHe(), partialAlveolarHe, time,
                                      cp.getConstants().heliumHalfTime));

    //std::cout << "\tCPn P. N2: " << cp.getPressureN2() << std::endl;
    //std::cout << "\tCPn P. He: " << cp.getPressureHe() << std::endl;
}

void Model::updateCompartmentTransient(Compartment &cp) {
    throw "UNIMPLEMENTED";
}

float Model::updateHaldaneGas(float pt0, float initialAlveolarPressure, float time, float halfLife) {
	float k;

	k = (float) M_LN2 / halfLife;
	return pt0 + (initialAlveolarPressure - pt0) * (1.0f - exp(-k * time));
}

float Model::convertCompositeCeilingPressureToDepth(float compositeCeilingAtm)
{
    return (compositeCeilingAtm - SURFACE_PRESSURE) * 33.0f;
}

