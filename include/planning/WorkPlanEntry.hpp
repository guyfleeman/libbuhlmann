//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_WORKPLANENTRY_H
#define LIBBUHLMAN_WORKPLANENTRY_H

#include <iostream>
#include <memory>

#include "BreathingGas.hpp"

namespace buhlmann {
namespace planning {

typedef enum WorkPlanType {
	TRAVEL_ENTRY,
	DECO_TRAVEL_ENTRY,
	WORK_ENTRY,
} WorkPlanType_t;

class WorkPlanEntry {
public:
	explicit WorkPlanEntry(float depth, float duration, std::shared_ptr<BreathingGas> assignedGas);

	explicit WorkPlanEntry(std::string, std::shared_ptr<BreathingGas> assignedGas);

	explicit WorkPlanEntry(const WorkPlanEntry &wpe);

	virtual ~WorkPlanEntry();

	BreathingGas& getAssignedGas();

	void assignGas(std::shared_ptr<BreathingGas> gas);

	//std::ostream & operator<<(std::ostream & os) {
	//friend std::ostream & operator<<(std::ostream & os, WorkPlanEntry const & wpe);
	friend std::ostream & operator<<(std::ostream & os, const buhlmann::planning::WorkPlanEntry & wpe) {
		os << "WPE: ";
		os << "Depth: " << wpe.depth << " (" << wpe.startDepth << ", " << wpe.endDepth << "), ";
		os << "Time: " << wpe.bottomTime << " (" << wpe.departureTime << ") ";
		os << *wpe.assignedGas;

		return os;
	}


	float depth{0};
	float startDepth{0};
	float endDepth{0};

	float arrivalTime{0};
	float departureTime{0};
	float bottomTime{0};

	WorkPlanType_t type = WORK_ENTRY;
private:

	std::shared_ptr<BreathingGas> assignedGas;

};
};
};

#endif //LIBBUHLMAN_WORKPLANENTRY_H
