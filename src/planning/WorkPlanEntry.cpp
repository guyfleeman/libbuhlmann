//
// Created by guyfleeman on 11/27/20.
//

#include <iostream>
#include <sstream>
#include <vector>

#include "WorkPlanEntry.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;
using namespace std;

WorkPlanEntry::WorkPlanEntry(const WorkPlanEntry &wpe) {
	depth = wpe.depth;
	startDepth = wpe.startDepth;
	endDepth = wpe.endDepth;

	bottomTime = wpe.bottomTime;
	arrivalTime = wpe.arrivalTime;
	departureTime = wpe.departureTime;

	type = wpe.type;

	assignedGas = wpe.assignedGas;
}

WorkPlanEntry::WorkPlanEntry(float depth, float duration, shared_ptr<buhlmann::BreathingGas> assignedGas) {
	this->depth = depth;
	this->startDepth = depth;
	this->endDepth = depth;

	this->bottomTime = duration;
	this->departureTime = duration;

	this->assignedGas = assignedGas;
}

WorkPlanEntry::WorkPlanEntry(std::string initStr, shared_ptr<buhlmann::BreathingGas> assignedGas) {

	vector<string> components;
	istringstream f(initStr);
	string s;
	while (getline(f, s, ' ')) {
		components.push_back(s);
	}

	if (components.size() != 2) {
		throw "WorkPlanEntry.cpp - ERROR invalid specification string. Format is <DEPTH> <TIME M>";
	}

	float depth = std::stof(components[0]);
	components[1].erase(components[1].end() - 1);
	float time = std::stof(components[1]);

	this->depth = depth;
	this->startDepth = depth;
	this->endDepth = depth;

	this->bottomTime = time * SECONDS_PER_MINUTE;
	this->departureTime = bottomTime;

	this->assignedGas = assignedGas;
}

WorkPlanEntry::~WorkPlanEntry() {}

void WorkPlanEntry::assignGas(shared_ptr<buhlmann::BreathingGas> gas) {
	this->assignedGas = gas;
}

/*
std::ostream & operator<<(std::ostream & os, WorkPlanEntry const & wpe) {
	os << "WPE: ";
	os << "Depth: " << wpe.depth << " (" << wpe.startDepth << ", " << wpe.endDepth << "), ";
	os << "Time: " << wpe.bottomTime << " (" << wpe.arrivalTime << ", " << wpe.departureTime << ")";

	return os;
}
 */

