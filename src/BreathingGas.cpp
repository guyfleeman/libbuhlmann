//
// Created by guyfleeman on 11/27/20.
//

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Units.hpp"

#include "BreathingGas.hpp"


using namespace buhlmann;

BreathingGas::BreathingGas(const buhlmann::BreathingGas &bg) {
	initializeFromRaw(bg.getFracO2(), bg.getFracHe(), bg.getFracH2());
	this->diverCtx = bg.diverCtx;
}

BreathingGas::BreathingGas(const float fracO2, float const fracHe, const float fracH2, std::shared_ptr<DiverParameters> ctx) {
	initializeFromRaw(fracO2, fracHe, fracH2);
	this->diverCtx = ctx;
}

BreathingGas::BreathingGas(std::string cRep, shared_ptr<buhlmann::DiverParameters> ctx) {
	//std::replace(cRep.begin(), cRep.end(), '\r', '');

	vector<string> components;
	istringstream f(cRep);
	string s;
	while (getline(f, s, ' ')) {
		components.push_back(s);
	}

	if (components.size() != 3 && components.size() != 4) {
		throw "BreathingGas.cpp - ERROR invalid specification string. Format is <TG | BG | DG | BAILOUT> <FRAC O2> <FRAC He> <? FRAC H2>";
	}

	if (components[0] == "TG") {
		gasType = TRAVEL_GAS;
	} else if (components[0] == "BG") {
		gasType = BACK_GAS;
	} else if (components[0] == "DG") {
		gasType = DECO_GAS;
	} else {
		throw "BreathingGas.cpp - ERROR invalid gas specifier";
	}

	float fracO2 = std::stof(components[1]) / 100.0f;
	float fracHe = 0.0f;
	float fracH2 = 0.0f;
	if (components.size() == 3) {
		components[2].erase(components[2].end() - 1);
		fracHe = std::stof(components[2]) / 100.0f;
	}
	if (components.size() == 4) {
		fracHe = std::stof(components[2]);
		components[3].erase(components[3].end() - 1);
		fracH2 = std::stof(components[3]) / 100.0f;
	}

	initializeFromRaw(fracO2, fracHe, fracH2);

	this->diverCtx = ctx;
}

void BreathingGas::initializeFromRaw(const float fracO2, const float fracHe, const float fracH2) {
	if (fracO2 < 0 || fracO2 > 1.0) {
		throw "Gas fraction must be between 0.0 and 1.0";
	}

	if (fracO2 < MINIMUM_SAFE_PPO2) {
		std::cout << "BreathingGas.cpp: WARNING - hypoxic mix in use, lethal surface gas blend in use";
	}

	if (fracHe < 0 || fracHe > 1.0) {
		throw "Gas fraction must be between 0.0 and 1.0";
	}

	if (fracH2 < 0 || fracH2 > 1.0) {
		throw "Gas fraction must be between 0.0 and 1.0";
	}

	if (fracO2 + fracHe + fracH2 > 1.0) {
		throw "Gas fractions cannot be above 1.0";
	}

	this->fracO2 = fracO2;
	this->fracHe = fracHe;
	this->fracH2 = fracH2;

	this->fracN2 = 1.0f - fracO2 - fracHe - fracH2;
}

BreathingGas::~BreathingGas() {}

float BreathingGas::getFracO2() const {
	return fracO2;
}

float BreathingGas::getFracHe() const {
	return fracHe;
}

float BreathingGas::getFracH2() const {
	return fracH2;
}

float BreathingGas::getFracN2() const {
	return fracN2;
}

float BreathingGas::getPPO2(float d) {
	return fracO2 * getTotalPP(d);
}

float BreathingGas::getPPHe(float d) {
	return fracHe * getTotalPP(d);
}

float BreathingGas::getPPH2(float d) {
	return fracH2 * getTotalPP(d);
}

float BreathingGas::getPPN2(float d) {
	return fracN2 * getTotalPP(d);
}

float BreathingGas::getMinimumOperatingDepth() {
	if (fracO2 >= MINIMUM_SAFE_PPO2) {
		return 0;
	}

	float surfacePPO2 = getFracO2() * surfaceATM;
	float atmDel = MINIMUM_SAFE_PPO2 - surfacePPO2;
	return atmDel / ATM_PER_FT_H20;
}

float BreathingGas::getMaximumOperatingDepth() {
	const float MAX_PPO2 = (gasType == DECO_GAS) ? diverCtx->getMaxDecoPPO2() : diverCtx->getMaxPPO2();
	return ((MAX_PPO2 / fracO2) - surfaceATM) * FT_H2O_PER_ATM;
}

float BreathingGas::getEquivalentNarcosisDepth(float depth) {
	return (depth + (FT_H2O_PER_ATM * surfaceATM)) * (1 - getFracHe() - getFracH2()) - FT_H2O_PER_ATM;
}

float BreathingGas::getTotalPP(float depth) {
	return surfaceATM + (depth / FT_H2O_PER_ATM);
}

GasType_t BreathingGas::getGasType() {
	return gasType;
}


