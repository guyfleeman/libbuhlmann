//
// Created by guyfleeman on 11/27/20.
//

#ifndef LIBBUHLMAN_BUHLMANNMODEL_HPP
#define LIBBUHLMAN_BUHLMANNMODEL_HPP

//#include <memory>
#include <list>

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
	Model(BuhlmannModelVersion version, DiverParameters &ctx);

	void initialize();

	void clearModel();

	void runForWorkPlan(planning::WorkPlan &wp);

	void runForWorkPlanEntry(planning::WorkPlanEntry &wpe);

	void runExplicitLogForWorkPlan(planning::WorkPlan &wp);

	float getCompositeCeilingAtm();

	static float convertCompositeCeilingPressureToDepth(float compositeDepthAtm);

    static float ambientPressureFromDepth(float d);

	float getCompositeNoDecompressionTime();
protected:
    void generateDecompressionSchedule(planning::WorkPlan &wp);

    void setGradientFactorSlope(float startStopDepth, float endStopDepth);

    float gradientFactor(float currentStopDepth);

	static void updateCompartmentStatic(float depth, float time, Compartment &cp, const BreathingGas &bg);

	void updateCompartmentTransient(Compartment &cp);

	static float updateLowLevelDiffusion_Depth(float depth, float rq, float internalGasRatio);

	static float updateLowLevelDiffusion(float ambientPressure, float rq, float internalGasRatio);
private:
	static float updateHaldaneGas(float pt0, float initialAlveolarPressure, float time, float halfLife);

	BuhlmannModelVersion algorithmVersion = BuhlmannModelVersion::ZHL_16C;

	DiverParameters diverCtx;

	std::array<Compartment, 17> compartments;

	float gradientFactorSlope = 1.0f;
};

};
};


#endif //LIBBUHLMAN_BUHLMANNMODEL_HPP
