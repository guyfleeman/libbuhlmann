//
// Created by guyfleeman on 11/28/20.
//

#include <cmath>

#include "BuhlmannModel.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;
using namespace buhlmann::model;

Model::Model(buhlmann::model::BuhlmannModelVersion version, shared_ptr<buhlmann::DiverParameters> ctx) {

}

void Model::clearModel() {

}

float Model::getCompositeCeiling() {

}

void Model::runForWorkPlan(shared_ptr<buhlmann::planning::WorkPlan> wp) {
	for (auto & wpe : wp->travelEntries) {
		runForWorkPlanEntry(wpe);
	}
}

void Model::runForWorkPlanEntry(WorkPlanEntry &wpe) {

}

float Model::updateLowLevelDiffusion(float ambientPressure, float rq, float igRatio) {
	return (ambientPressure - WATER_VAPOR_PRESSURE + ((1 - rq) / rq) * CO2_PRESSURE) * igRatio;
}

void Model::updateCompartmentStatic(Compartment &cp) {
	double n2_palv;
	double he_palv;

	n2_palv = updateLowLevelDiffusion(, rq, n2_ratio);
	he_palv = ventilation(p_ambient, rq, he_ratio);

	end->n2_p = updateHaldaneGas(cp.pressureN2, n2_palv, time,
	                    constants->n2_h);
	end->he_p = updateHaldaneGas(cp.pressureHe, he_palv, time,
	                    constants->he_h);

}

void Model::updateCompartmentTransient(Compartment &cp) {

}

float Model::updateHaldaneGas(float pt0, float initialAlveolarPressure, float time, float halfLife) {
	float k;

	k = (float) M_LN2 / halfLife;
	return pt0 + (initialAlveolarPressure - pt0) * (1.0f - exp(-k * time));
}
