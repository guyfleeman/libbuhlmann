//
// Created by guyfleeman on 11/28/20.
//

#include <cmath>
#include <utility>

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
    std::cout << "Initializing State" << std::endl;

    if (algorithmVersion == BuhlmannModelVersion::ZHL_16C) {
        for (size_t i = 0; i < ZHL_16C_COMPARTMENTS; i++) {
            compartments[i].setPressureN2(updateLowLevelDiffusion_Depth(0.0f, BUHLMANN_RQ, SURFACE_PCT_N2));
            compartments[i].setPressureHe(updateLowLevelDiffusion_Depth(0.0f, BUHLMANN_RQ, SURFACE_PCT_HE));
            std::cout << compartments[i] << std::endl;
        }

        std::cout << std::endl;
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
    std::cout << "Round Ceiling: " << convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm()) << std::endl;
    //std::cout << "Round NDL: " << getCompositeNoDecompressionTime() << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;
}

void Model::generateDecompressionSchedule(WorkPlan &wp)
{
    const float SURFACE_THRESHOLD = 2.5f;
    const float STOP_DEPTH_INTERVAL = 10.0f;
    const float LAST_STOP_DEPTH = 10.0f;
    const float STOP_TIME_INTERVAL = 60.0f;

    float currentCeiling = convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm());
    float currentDescritizedCeilingTarget = 0.0f;
    float nextDescritizedCeilingTarget = 0.0f;

    while (currentCeiling >= SURFACE_THRESHOLD) {
        currentCeiling = convertCompositeCeilingPressureToDepth(getCompositeCeilingAtm());
        currentDescritizedCeilingTarget = floor(currentCeiling / STOP_DEPTH_INTERVAL) * STOP_DEPTH_INTERVAL + STOP_DEPTH_INTERVAL;
        nextDescritizedCeilingTarget = currentDescritizedCeilingTarget - STOP_DEPTH_INTERVAL;
        if (nextDescritizedCeilingTarget < LAST_STOP_DEPTH) {
            nextDescritizedCeilingTarget = LAST_STOP_DEPTH;
        }

        // pick a gas
        auto &bg = const_cast<BreathingGas &>(wp.getOptimizedGasForDepth(currentDescritizedCeilingTarget));
        std::cout << "chose optimized gas: " << bg << std::endl;
        std::cout << "targeting " << currentDescritizedCeilingTarget;

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

        // this terminating condition is straight forward and should be self-documenting
        for(iterations = 1; (currentDescritizedCeilingTarget != LAST_STOP_DEPTH && currentCeiling > nextDescritizedCeilingTarget)
        || (currentDescritizedCeilingTarget == LAST_STOP_DEPTH && currentCeiling > SURFACE_THRESHOLD); iterations++) {
            // run plan 60 STOP_TIME_INTERVAL at a time
            float updatedCeilingAtm = 0.0f;
            for (auto & cp : compartmentsScratchSpace) {
                updateCompartmentStatic(decoEntry.depth, decoEntry.bottomTime, cp, decoEntry.getAssignedGas());
                updatedCeilingAtm = max(cp.getCeiling(), updatedCeilingAtm);
            }

            std::cout << "ran depth " << decoEntry.depth << " for " << (float) iterations * STOP_TIME_INTERVAL << std::endl;
            //std::cout << "new ceiling ATM: " << updatedCeilingAtm << std::endl;
            float updatedCeiling = convertCompositeCeilingPressureToDepth(updatedCeilingAtm);
            std::cout << "new ceiling: " << updatedCeiling << std::endl;

            currentCeiling = updatedCeiling;
        }

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
    return updateLowLevelDiffusion(ambientPressureFromDepth(depth), rq, internalGasRatio);
}

float Model::updateLowLevelDiffusion(float ambientPressure, float rq, float internalGasRatio) {
	return (ambientPressure - WATER_VAPOR_PRESSURE + ((1 - rq) / rq) * CO2_PRESSURE) * internalGasRatio;
}

void Model::updateCompartmentStatic(float depth, float time, Compartment &cp, const BreathingGas &bg)
{
	float partialAlveolarN2, partialAlveolarHe;

    partialAlveolarN2 = updateLowLevelDiffusion_Depth(depth, BUHLMANN_RQ, bg.getFracN2());
    partialAlveolarHe = updateLowLevelDiffusion_Depth(depth, BUHLMANN_RQ, bg.getFracHe());
    //std::cout << "\tLL P. Alv N2: " << partialAlveolarN2 << std::endl;
    //std::cout << "\tLL P. Alv He: " << partialAlveolarHe << std::endl;

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

