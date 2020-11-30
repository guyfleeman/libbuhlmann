//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_DIVERPARAMETERS_HPP
#define LIBBUHLMAN_DIVERPARAMETERS_HPP

#include <string>

#include "units/Units.hpp"

using namespace std;

namespace buhlmann {
enum O2EndMode {
	O2_NARCOTIC,
	O2_NON_NARCOTIC
};

class DiverParameters {
public:
	DiverParameters(const float maxPPO2Std, const float maxPPO2Deco,
	                const float descentRate, const float ascentRate,
					const float gradientFactorLo = 35.0f, const float gradientFactorHi = 85.0f);

    DiverParameters();

    virtual ~DiverParameters();

	float getMaxPPO2();

	float getMaxDecoPPO2();

	float getAscentRate();

	float getDescentRate();

	float getGradientFactorLo();

	float getGradientFactorHi();

	void loadFromFile(std::string file) {

	}

private:
	std::string name;

	float maxPPO2Std{};
	float maxPPO2Deco{};
	float descentRate{};
	float ascentRate{};
	float gradientFactorLo{};
	float gradientFactorHi{};
	O2EndMode o2EndMode = O2_NARCOTIC;
};
}


#endif //LIBBUHLMAN_DIVERPARAMETERS_HPP
