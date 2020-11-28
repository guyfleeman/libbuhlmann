//
// Created by guyfleeman on 11/26/2020.
//

#include <memory>

#include "DiverParameters.hpp"
#include "WorkPlan.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;
using namespace std;

int main() {
	std::shared_ptr<DiverParameters> dp = std::make_shared<DiverParameters>(1.4f, 1.6f, 60.0f, 20.0f);
	WorkPlan wp = WorkPlan(dp);
	wp.loadFromFile("../data/dp1.txt");
	wp.contextualize();


}