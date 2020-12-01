//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_WORKPLAN_HPP
#define LIBBUHLMAN_WORKPLAN_HPP

#include <iostream>
#include <vector>

#include "DiverParameters.hpp"
#include "WorkPlanEntry.hpp"

namespace buhlmann {
namespace planning {

/**
 * A composite of depths and times over which underwater work will be done.
 */
class WorkPlan {
public:
    /**
     * Creates a work plan, contextualized to a specific individual or team represented by ctx
     * @param ctx paramteres that constrain a generalized plan to an individual or group
     */
	explicit WorkPlan(DiverParameters &ctx);

	/**
	 * destructor
	 */
	virtual ~WorkPlan();

	/**
	 * initializes the work plan from a file
	 * @param file filename
	 */
	void loadFromFile(std::string file);

	/**
	 * sets the available breathing gasses
	 * @param gs gasses
	 */
	void setGasses(std::vector<BreathingGas> gs);

	/**
	 * finds the most optimal gas for a given depth
	 * gasses must have a acceptable MOD and PPO2
	 * prefer high percentages of oxygen
	 * prefer Helium over Nitrogen when END is nearing a designated threshold
	 * @param d depth to optimize for
	 * @return gas
	 */
	const BreathingGas& getOptimizedGasForDepth(float d);

	/**
	 * loads the gas database from a file
	 * @param filename
	 */
	void loadGassesFromFile(std::string filename);

	/**
	 * modifies the work plan to fit the human constraints of the team
	 */
	void contextualize();

	/**
	 * adds entries for non-trivial travel segments between work depths
	 */
	void resolveTravelIntermediates();

	/**
	 * automatically assigns gasses to specific work segments
	 */
	void assignGasses();

	/**
	 * prints the workplan
	 * @param os
	 * @param wp
	 * @return
	 */
	friend std::ostream & operator<<(std::ostream & os, const buhlmann::planning::WorkPlan & wp) {
		for (auto & wpe : wp.travelEntries) {
			os << wpe << std::endl;
		}

		return os;
	}

	/**
	 * contextualized work plan segments
	 */
	std::vector<WorkPlanEntry> travelEntries;

private:
    /**
     * backing context
     */
	DiverParameters diverCtx;

	/**
	 * default non-contextualized entries
	 */
	std::vector<WorkPlanEntry> userEntries;

	/**
	 * available gasses
	 */
	std::vector<BreathingGas> gasSchedule;
};
}
}

#endif //LIBBUHLMAN_WORKPLAN_HPP
