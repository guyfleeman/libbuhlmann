//
// Created by guyfleeman on 11/26/2020.
//

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <omp.h>

#include "DiverParameters.hpp"

#include "WorkPlan.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;

WorkPlan::WorkPlan(DiverParameters &ctx) {
	this->diverCtx = ctx;
}

WorkPlan::~WorkPlan() {}

void WorkPlan::loadFromFile(std::string filename) {
	std::ifstream infile(filename);
	std::string line;

	while (std::getline(infile, line))
	{
		userEntries.emplace_back(line, nullptr);
	}

	#pragma omp parallel num_threads(std::thread::hardware_concurrency() / 2)
	for (size_t i = 0; i < userEntries.size() - 1; i++) {
		userEntries[i + 1].arrivalTime = userEntries[i].departureTime;
		userEntries[i + 1].departureTime = userEntries[i + 1].arrivalTime + userEntries[i + 1].bottomTime;
	}
}

void WorkPlan::setGasses(vector<BreathingGas> gs) {

}

void WorkPlan::loadGassesFromFile(std::string filename) {
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		BreathingGas bg(line, diverCtx);
		gasSchedule.emplace_back(bg);
		//gasSchedule.emplace_back(line, diverCtx);
	}
}

void WorkPlan::contextualize() {
	resolveTravelIntermediates();
	assignGasses();
}

void WorkPlan::resolveTravelIntermediates() {
	float ascentRate = diverCtx.getAscentRate();
	float descentRate = diverCtx.getDescentRate();

	// calculate travel times, and adjust entries and intermediate depths for on gas calculations
	const float TRAVEL_TIME_THRESH_S = 30;
	std::shared_ptr<WorkPlanEntry> prevEntry = std::make_shared<WorkPlanEntry>(0.0f, 0.0f, nullptr);
	
	#pragma omp parallel num_threads(std::thread::hardware_concurrency() / 2)
	for (auto it = userEntries.begin(); it != userEntries.end(); it++) {
		float deltaDepth = prevEntry->depth - it->depth;
		float transitionTimeS = 0.0f;
		if (deltaDepth < 0) {
			transitionTimeS = abs(deltaDepth) / diverCtx.getDescentRate() * SECONDS_PER_MINUTE;
		} else {
			transitionTimeS = deltaDepth / diverCtx.getAscentRate() * SECONDS_PER_MINUTE;
		}

		float depthApprox = (prevEntry->depth + it->depth) / 2.0f;
		WorkPlanEntry transitionEntry(depthApprox, transitionTimeS, std::shared_ptr<BreathingGas>(nullptr));
		transitionEntry.type = TRAVEL_ENTRY;
		transitionEntry.startDepth = prevEntry->depth;
		transitionEntry.endDepth = it->depth;

		// update times
		transitionEntry.arrivalTime = prevEntry->departureTime;
		transitionEntry.departureTime = transitionEntry.arrivalTime + transitionEntry.bottomTime;

		it->arrivalTime += transitionEntry.bottomTime;
		it->bottomTime -= transitionEntry.bottomTime;

		// add entry
		travelEntries.emplace_back(transitionEntry);
		travelEntries.emplace_back(*it);

		prevEntry = std::make_shared<WorkPlanEntry>(*it);
	}
}

void WorkPlan::assignGasses() {
	std::shared_ptr<BreathingGas> bg;
	for (auto it = gasSchedule.begin(); it != gasSchedule.end(); it++) {
		if (it->getGasType() == BACK_GAS) {
			bg = std::make_shared<BreathingGas>(*it);
			break;
		}
	}

	if (bg == nullptr) {
		throw "WorkPlan.cpp: ERROR - no primary gas found";
	}

	for (auto & entry : travelEntries) {
		if (entry.depth > bg->getMaximumOperatingDepth()) {
			throw "WorkPlan.cpp: ERROR - gas in unsuitable for work plan";
		}

		entry.assignGas(bg);
	}
}

const BreathingGas& WorkPlan::getOptimizedGasForDepth(float d)
{
    BreathingGas &curBest(gasSchedule.front());
    for (BreathingGas & bg : gasSchedule) {
        if (bg.getMaximumOperatingDepth() < d) {
            continue;
        }

        if (bg.getFracO2() > curBest.getFracO2()) {
            curBest = bg;
        }
    }

    return curBest;
}

/*
std::ostream & operator<<(std::ostream & os, WorkPlan const & wp) {
	for (auto & wpe : wp.travelEntries) {
		os << wpe << std::endl;
	}

	return os;
}
*/
