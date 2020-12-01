//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_DIVERPARAMETERS_HPP
#define LIBBUHLMAN_DIVERPARAMETERS_HPP

#include <string>

#include "units/Units.hpp"

using namespace std;

namespace buhlmann {
/**
 * O2 narcosis interpretation
 */
enum O2EndMode {
	O2_NARCOTIC,
	O2_NON_NARCOTIC
};

/**
 * represents a diver or team
 */
class DiverParameters {
public:
    /**
     * creates a context
     * @param maxPPO2Std maximum acceptable partial pressure of Oxygen under high work load
     * @param maxPPO2Deco maximum acceptable partial pressure of Oxygen at rest (deco)
     * @param descentRate descent rate in ft/min
     * @param ascentRate ascent rate in ft/min
     * @param gradientFactorLo gradient factor lo pct
     * @param gradientFactorHi gradient factor hi pct
     */
	DiverParameters(const float maxPPO2Std, const float maxPPO2Deco,
	                const float descentRate, const float ascentRate,
					const float gradientFactorLo = 35.0f, const float gradientFactorHi = 85.0f);

	/**
	 * default constructor
	 */
    DiverParameters();

    /**
     * destructor
     */
    virtual ~DiverParameters();

    /**
     * gets the max PPO2
     * @return
     */
	float getMaxPPO2();

	/**
	 * gets the max deco PPO2
	 * @return
	 */
	float getMaxDecoPPO2();

	/**
	 * gets the ascent rate
	 * @return
	 */
	float getAscentRate();

	/**
	 * gets the descent rate
	 * @return
	 */
	float getDescentRate();

	/**
	 * gets the low gradient factor
	 * @return
	 */
	float getGradientFactorLo();

	/**
	 * gets the high gradient factor
	 * @return
	 */
	float getGradientFactorHi();

	void loadFromFile(std::string file) {

	}

private:
	std::string name;

	/**
	 *
	 */
	float maxPPO2Std{};

	/**
	 *
	 */
	float maxPPO2Deco{};

	/**
	 *
	 */
	float descentRate{};

	/**
	 *
	 */
	float ascentRate{};

	/**
	 *
	 */
	float gradientFactorLo{};

	/**
	 *
	 */
	float gradientFactorHi{};

	/**
	 *
	 */
	O2EndMode o2EndMode = O2_NARCOTIC;
};
}


#endif //LIBBUHLMAN_DIVERPARAMETERS_HPP
