//
// Created by guyfleeman on 10/17/2020.
//

#include "DiverParameters.hpp"

using namespace buhlmann;

DiverParameters::DiverParameters(const float maxPPO2Std, const float maxPPO2Deco,
								 const float descentRate, const float ascentRate,
								 const float gradientFactorLo, const float gradientFactorHi) {
	this->maxPPO2Std = maxPPO2Std;
	this->maxPPO2Deco = maxPPO2Deco;
	this->descentRate = descentRate;
	this->ascentRate = ascentRate;
	this->gradientFactorLo = gradientFactorLo;
	this->gradientFactorHi = gradientFactorHi;
}

DiverParameters::~DiverParameters() {}

float DiverParameters::getAscentRate() {
	return ascentRate;
}

float DiverParameters::getDescentRate() {
	return descentRate;
}

float DiverParameters::getMaxPPO2() {
	return maxPPO2Std;
}

float DiverParameters::getMaxDecoPPO2() {
	return maxPPO2Deco;
}

float DiverParameters::getGradientFactorLo() {
	return gradientFactorLo;
}

float DiverParameters::getGradientFactorHi() {
	return gradientFactorHi;
}

DiverParameters::DiverParameters()
{

}
