//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_WORKPLAN_HPP
#define LIBBUHLMAN_WORKPLAN_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "DiverParameters.hpp"
#include "WorkPlanEntry.hpp"

namespace buhlmann {
namespace planning {
class WorkPlan {
public:
	explicit WorkPlan(std::shared_ptr<DiverParameters> ctx);

	virtual ~WorkPlan();

	void loadFromFile(std::string file);

	void setGasses(std::vector<BreathingGas> gs);

	void loadGassesFromFile(std::string filename);

	void contextualize();

	void resolveTravelIntermediates();

	void assignGasses();

	//friend std::ostream & operator<<(std::ostream & os, WorkPlan const & wp);
	friend std::ostream & operator<<(std::ostream & os, const buhlmann::planning::WorkPlan & wp) {
		for (auto & wpe : wp.travelEntries) {
			os << wpe << std::endl;
		}

		return os;
	}

	std::vector<WorkPlanEntry> travelEntries;

private:
	std::shared_ptr<DiverParameters> diverCtx;

	std::vector<WorkPlanEntry> userEntries;
	std::vector<WorkPlanEntry> completedEntries;

	std::vector<BreathingGas> gasSchedule;
};
}
}

#endif //LIBBUHLMAN_WORKPLAN_HPP
