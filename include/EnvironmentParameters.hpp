//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_ENVIRONMENTPARAMETERS_HPP
#define LIBBUHLMAN_ENVIRONMENTPARAMETERS_HPP

#include "units/Units.hpp"

namespace libbuhlman {
class EnvironmentParameters {
public:
	EnvironmentParameters(Pressure surfacePressure, Temperature surfaceTemperature);
	virtual ~EnvironmentParameters();
};
}

#endif //LIBBUHLMAN_ENVIRONMENTPARAMETERS_HPP
