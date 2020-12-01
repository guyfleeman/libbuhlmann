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

/**
 * work plan type
 */
typedef enum WorkPlanType {
	TRAVEL_ENTRY,
	DECO_TRAVEL_ENTRY,
	WORK_ENTRY,
} WorkPlanType_t;

/**
 * work plan entry
 */
class WorkPlanEntry {
public:
    /**
     * default constructor
     * @param depth depth of the entry in feet
     * @param duration bottom time of the entry in seconds
     * @param assignedGas breathing gas assigned to the entry
     */
	explicit WorkPlanEntry(float depth, float duration, std::shared_ptr<BreathingGas> assignedGas);

	/**
	 * creates an empty entry, used for scratch work in intermediates
	 * @param assignedGas
	 */
	explicit WorkPlanEntry(std::string, std::shared_ptr<BreathingGas> assignedGas);

	/**
	 * copy constructor
	 * @param wpe
	 */
	explicit WorkPlanEntry(const WorkPlanEntry &wpe);

	/**
	 * destructor
	 */
	virtual ~WorkPlanEntry();

	/**
	 * gets the assigned gas
	 * @return
	 */
	BreathingGas& getAssignedGas();

	/**
	 * assigns a gas
	 * @param gas
	 */
	void assignGas(std::shared_ptr<BreathingGas> gas);

	/**
	 * prints the entry
	 * @param os
	 * @param wpe
	 * @return
	 */
	friend std::ostream & operator<<(std::ostream & os, const buhlmann::planning::WorkPlanEntry & wpe) {
		os << "WPE: ";
		os << "Depth: " << wpe.depth << " (" << wpe.startDepth << ", " << wpe.endDepth << "), ";
		os << "Time: " << wpe.bottomTime << " (" << wpe.departureTime << ") ";
		os << *wpe.assignedGas;

		return os;
	}

    /**
     * average depth
     */
	float depth{0};

	/**
	 * start depth
	 */
	float startDepth{0};

	/**
	 * end depth
	 */
	float endDepth{0};

	/**
	 * time of arrival in seconds
	 */
	float arrivalTime{0};

	/**
	 * time of departure in seconds
	 */
	float departureTime{0};

	/**
	 * bottom time in seconds
	 */
	float bottomTime{0};

	/**
	 * entry type
	 */
	WorkPlanType_t type = WORK_ENTRY;
private:
    /**
     * assigned gas
     */
	std::shared_ptr<BreathingGas> assignedGas;
};
}
}

#endif //LIBBUHLMAN_WORKPLANENTRY_H
