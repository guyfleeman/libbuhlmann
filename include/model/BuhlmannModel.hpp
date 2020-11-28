//
// Created by guyfleeman on 11/27/20.
//

#ifndef LIBBUHLMAN_BUHLMANNMODEL_HPP
#define LIBBUHLMAN_BUHLMANNMODEL_HPP

#include <memory>

namespace buhlmann {
namespace model {

typedef enum BuhlmannModelVersion {
	ZHL_16C
} BuhlmannModelVersion_t;

class Compartment {

};

class BuhlmannModel {
	BuhlmannModel(BuhlmannModelVersion_t version, std::shared_ptr<DiverParameters> ctx);

	void clearModel();

	void runForWorkPlan(std::shared_ptr<WorkPlan> wp);
};

}
}


#endif //LIBBUHLMAN_BUHLMANNMODEL_HPP
