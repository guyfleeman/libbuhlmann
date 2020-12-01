//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_BREATHINGGAS_HPP
#define LIBBUHLMAN_BREATHINGGAS_HPP

#include <iostream>

#include "Units.hpp"
#include "DiverParameters.hpp"

namespace buhlmann {

/**
 * breathing gas type
 */
typedef enum GasType {
	TRAVEL_GAS,
	BACK_GAS,
	DECO_GAS,
	BAILOUT_GAS,

} GasType_t;

/**
 * represents a breathing gas
 */
class BreathingGas {
public:
    /**
     * creates a breathing gas
     * @param ctx diver context
     * @param fracO2 fraction of Oxygen
     * @param fracHe fraction of Helium
     * @param fracH2 fraction of Hydrogen
     */
	explicit BreathingGas(DiverParameters &ctx, const float fracO2, const float fracHe = 0, const float fracH2 = 0);

	/**
	 * creates a breathing gas from an init string
	 * @param cRep
	 * @param ctx
	 */
	explicit BreathingGas(std::string cRep, DiverParameters &ctx);

	/**
	 * copy constructor
	 * @param bg
	 */
	explicit BreathingGas(const BreathingGas &bg);

	/**
	 * destructor
	 */
	virtual ~BreathingGas();

	/**
	 *
	 * @return
	 */
	float getFracN2() const;

	/**
	 *
	 * @return
	 */
	float getFracO2() const;

	/**
	 *
	 * @return
	 */
	float getFracHe() const;

	/**
	 *
	 * @return
	 */
	float getFracH2() const;

	/**
	 *
	 * @param d
	 * @return
	 */
	float getPPN2(float d);

	/**
	 *
	 * @param d
	 * @return
	 */
	float getPPO2(float d);

	/**
	 *
	 * @param d
	 * @return
	 */
	float getPPHe(float d);

	/**
	 *
	 * @param d
	 * @return
	 */
	float getPPH2(float d);

	/**
	 * calculate minimum operating depth for hypoxic mixes
	 * @return
	 */
	float getMinimumOperatingDepth();

	/**
	 * get maximum operating depth based on context, work load, type and PPO2
	 * @return
	 */
	float getMaximumOperatingDepth();

	/**
	 * calculate END with O2 as non-narcotic
	 * @param depth
	 * @return
	 */
	float getEquivalentNarcosisDepth(float depth);

	/**
	 *
	 * @param os
	 * @param wpe
	 * @return
	 */
	friend std::ostream & operator<<(std::ostream & os, const buhlmann::BreathingGas & wpe) {
		if (wpe.getFracO2() == 21.0f && wpe.getFracHe() == 0.0f) {
			os << "Breathing Gas: Air";
		} else if (wpe.getFracHe() == 0.0f) {
			os << "Breathing Gas: EAN" << (wpe.getFracO2() * 100.0f);
		} else {
			os << "Breathing Gas: TMX " << (wpe.getFracO2() * 100.0f) << "/" << (wpe.getFracHe() * 100.0f);
		}

		return os;
	}

	/**
	 * gets the type
	 * @return
	 */
	GasType_t getGasType();

	/**
	 * baseline atm at surface
	 */
	static constexpr float surfaceATM = 1.0f;

	/**
	 * total PP at depth
	 * @param d
	 * @return
	 */
	static float getTotalPP(float d);

private:
    /**
     * assist constructor chaining
     * @param fracO2
     * @param fracHe
     * @param fracH2
     */
	void initializeFromRaw(const float fracO2, const float fracHe, const float fracH2);

	/**
	 * fraction N2
	 */
	float fracN2;

	/**
	 * fraction O2
	 */
	float fracO2;

	/**
	 * fraction He
	 */
	float fracHe;

	/**
	 * fraction H2
	 */
	float fracH2;

	/**
	 *
	 */
	DiverParameters diverCtx = DiverParameters(0, 0, 0, 0);

	/**
	 *
	 */
	GasType_t gasType = BACK_GAS;
};
}

#endif //LIBBUHLMAN_BREATHINGGAS_HPP
