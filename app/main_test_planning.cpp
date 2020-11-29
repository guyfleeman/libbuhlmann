//
// Created by guyfleeman on 11/27/20.
//

#include <iostream>

#include "WorkPlan.hpp"
#include "DiverParameters.hpp"

using namespace buhlmann;
using namespace buhlmann::planning;

int main() {
	std::cout << "main test planning" << std::endl;

	const float MAX_DIVE_PPO2 = 1.4f;
	const float MAX_DECO_PPO2 = 1.6f;

	std::shared_ptr<buhlmann::DiverParameters> dp = std::make_shared<buhlmann::DiverParameters>(MAX_DIVE_PPO2, MAX_DECO_PPO2, 60.0f, 20.0f);
	buhlmann::planning::WorkPlan wp = buhlmann::planning::WorkPlan(dp);
	wp.loadFromFile("../data/dp1.txt");
	wp.loadGassesFromFile("../data/gs1.txt");
	wp.contextualize();
	std::cout << wp << std::endl;
}
