//
// Created by guyfleeman on 11/28/20.
//

#ifndef LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP
#define LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP

#include <memory>

#include "BreathingGas.hpp"
#include "BuhlmannModel.hpp"
#include "BuhlmannModelVersion.hpp"
#include "BuhlmannConstatnts.hpp"

namespace buhlmann {
namespace model {

class Model;

class Compartment {
	//friend class Model;

public:
	explicit Compartment(BuhlmannModelVersion version, int compartmentNum);

	Compartment(Compartment &compartment);

	~Compartment();

	void runFor(float time, float depth, std::shared_ptr<BreathingGas> bg, std::shared_ptr<Model> bhm);

	std::unique_ptr<Compartment> runForCopy(float time, float depth, std::shared_ptr<BreathingGas> bg, std::shared_ptr<Model> bhm);

	CompartmentConstants_t getConstants();

	float getCeiling();

	static CompartmentConstants_t getConstants(BuhlmannModelVersion version, int compartmentNum);

private:
	int compartmentNum = 16;

	BuhlmannModelVersion version = BuhlmannModelVersion::ZHL_16C;
	CompartmentConstants_t constants = getConstants(version, compartmentNum);

	float pressureN2{};
	float pressureHe{};
};
};
};
#endif //LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP
