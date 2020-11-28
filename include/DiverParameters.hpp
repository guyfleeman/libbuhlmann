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
	                const float descentRate, const float ascentRate);

	virtual ~DiverParameters();

	float getMaxPPO2();

	float getMaxDecoPPO2();

	float getAscentRate();

	float getDescentRate();

	void loadFromFile(std::string file) {

	}

private:
	std::string name;

	float maxPPO2Std{};
	float maxPPO2Deco{};
	float descentRate{};
	float ascentRate{};
	O2EndMode o2EndMode = O2_NARCOTIC;
};
}


#endif //LIBBUHLMAN_DIVERPARAMETERS_HPP
