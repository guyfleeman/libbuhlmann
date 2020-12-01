//
// Created by guyfleeman on 11/27/20.
//

#ifndef LIBBUHLMAN_BUHLMANNMODEL_HPP
#define LIBBUHLMAN_BUHLMANNMODEL_HPP

#include <list>

#include "BuhlmannCompartment.hpp"
#include "BuhlmannModelVersion.hpp"
#include "DiverParameters.hpp"
#include "WorkPlan.hpp"
#include "WorkPlanEntry.hpp"

namespace buhlmann {
namespace model {

class Compartment;

/**
 * represents the buhlmann model
 */
class Model {
	//friend class Compartment;
public:
    /**
     * creates a model
     * @param version version
     * @param ctx context
     */
	Model(BuhlmannModelVersion version, DiverParameters &ctx);

	/**
	 * initializes the model
	 */
	void initialize();

	/**
	 * clears the model
	 */
	void clearModel();

	/**
	 * runs for a work plan, generating intermediates and a decompression schedule
	 * @param wp
	 */
	void runForWorkPlan(planning::WorkPlan &wp);

	/**
	 * runs for a work plan entry
	 * @param wpe
	 */
	void runForWorkPlanEntry(planning::WorkPlanEntry &wpe);

	/**
	 * logs explicit intervals of data for a WorkPlan. Does not generate stages
	 * @param wp
	 */
	void runExplicitLogForWorkPlan(planning::WorkPlan &wp);

	/**
	 * gets the absolute ceiling in atm
	 * @return
	 */
	float getCompositeCeilingAtm();

	/**
	 * converts ceiling in atm to ceiling in ft
	 * @param compositeDepthAtm
	 * @return
	 */
	static float convertCompositeCeilingPressureToDepth(float compositeDepthAtm);

	/**
	 * gets ambient pressure at a depth
	 * @param d
	 * @return
	 */
    static float ambientPressureFromDepth(float d);

    /**
     * gets the no decompression time
     * @return
     */
	float getCompositeNoDecompressionTime();
protected:
    /**
     * generates a decompression schedule at the end of a work plan
     * @param wp
     */
    void generateDecompressionSchedule(planning::WorkPlan &wp);

    /**
     * sets the gradient factor
     * @param startStopDepth
     * @param endStopDepth
     */
    void setGradientFactorSlope(float startStopDepth, float endStopDepth);

    /**
     * gets the gradient factor correction at a depth
     * @param currentStopDepth
     * @return
     */
    float gradientFactor(float currentStopDepth);

    /**
     * updates a compartment
     * @param depth in ft
     * @param time in seconds
     * @param cp compartment
     * @param bg assigned gas
     */
	static void updateCompartmentStatic(float depth, float time, Compartment &cp, const BreathingGas &bg);

	/**
	 * workmann sub-model, UNUSED
	 * @param cp
	 */
	void updateCompartmentTransient(Compartment &cp);

	/**
	 * updates alveolar diffusion
	 * @param depth
	 * @param rq
	 * @param internalGasRatio
	 * @return
	 */
	static float updateLowLevelDiffusion_Depth(float depth, float rq, float internalGasRatio);

	/**
	 * updates alveolar diffusion
	 * @param ambientPressure
	 * @param rq
	 * @param internalGasRatio
	 * @return
	 */
	static float updateLowLevelDiffusion(float ambientPressure, float rq, float internalGasRatio);
private:
    /**
     * updates alveolar diffusion using the haldane gas model
     * @param pt0
     * @param initialAlveolarPressure
     * @param time
     * @param halfLife
     * @return
     */
	static float updateHaldaneGas(float pt0, float initialAlveolarPressure, float time, float halfLife);

	/**
	 * version
	 */
	BuhlmannModelVersion algorithmVersion = BuhlmannModelVersion::ZHL_16C;

	/**
	 * context
	 */
	DiverParameters diverCtx;

	/**
	 * compartments
	 */
	std::array<Compartment, 17> compartments;

	/**
	 * current calculated GF slope
	 */
	float gradientFactorSlope = 1.0f;
};

};
};


#endif //LIBBUHLMAN_BUHLMANNMODEL_HPP
