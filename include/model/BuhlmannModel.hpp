//
// Created by guyfleeman on 11/27/20.
//

#ifndef LIBBUHLMAN_BUHLMANNMODEL_HPP
#define LIBBUHLMAN_BUHLMANNMODEL_HPP

#include <memory>

#include "BuhlmannCompartment.hpp"
#include "BuhlmannModelVersion.hpp"
#include "DiverParameters.hpp"
#include "WorkPlan.hpp"
#include "WorkPlanEntry.hpp"

namespace buhlmann {
namespace model {

class Compartment;

class Model {
	//friend class Compartment;
public:
	Model(BuhlmannModelVersion version, std::shared_ptr<DiverParameters> ctx);

	void clearModel();

	void runForWorkPlan(std::shared_ptr<planning::WorkPlan> wp);

	void runForWorkPlanEntry(planning::WorkPlanEntry &wpe);

	float getCompositeCeiling();

protected:
	void updateCompartmentStatic(Compartment &cp);

	void updateCompartmentTransient(Compartment &cp);

	float updateLowLevelDiffusion(float ambientPressure, float rq, float igRatio);
private:
	float updateHaldaneGas(float pt0, float initialAlveolarPressure, float time, float halfLife);

	BuhlmannModelVersion algorithmVersion = BuhlmannModelVersion::ZHL_16C;

	std::shared_ptr<DiverParameters> diverCtx;

	std::vector<Compartment> compartments;
};

};
};


#endif //LIBBUHLMAN_BUHLMANNMODEL_HPP
