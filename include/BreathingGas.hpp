//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_BREATHINGGAS_HPP
#define LIBBUHLMAN_BREATHINGGAS_HPP

#include <iostream>
#include <memory>

#include "Units.hpp"
#include "DiverParameters.hpp"

namespace buhlmann {

typedef enum GasType {
	TRAVEL_GAS,
	BACK_GAS,
	DECO_GAS,
	BAILOUT_GAS,

} GasType_t;

class BreathingGas {
public:
	explicit BreathingGas(const float fracO2, const float fracHe = 0, const float fracH2 = 0, std::shared_ptr<DiverParameters> ctx = nullptr);

	explicit BreathingGas(std::string cRep, std::shared_ptr<DiverParameters> ctx);

	explicit BreathingGas(const BreathingGas &bg);

	virtual ~BreathingGas();

	float getFracN2() const;

	float getFracO2() const;

	float getFracHe() const;

	float getFracH2() const;

	float getPPN2(float d);

	float getPPO2(float d);

	float getPPHe(float d);

	float getPPH2(float d);

	float getMinimumOperatingDepth();

	float getMaximumOperatingDepth();

	float getEquivalentNarcosisDepth(float depth);

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

	GasType_t getGasType();

	static constexpr float surfaceATM = 1.0f;

	static float getTotalPP(float d);

private:
	void initializeFromRaw(const float fracO2, const float fracHe, const float fracH2);

	float fracN2;
	float fracO2;
	float fracHe;
	float fracH2;

	std::shared_ptr<DiverParameters> diverCtx;

	GasType_t gasType = BACK_GAS;
};
}

#endif //LIBBUHLMAN_BREATHINGGAS_HPP
