//
// Created by guyfleeman on 11/27/20.
//

#include <iostream>

#include "BuhlmannModel.hpp"
#include "BuhlmannModelVersion.hpp"
#include "WorkPlan.hpp"
#include "DiverParameters.hpp"

using namespace buhlmann;
using namespace buhlmann::model;
using namespace buhlmann::planning;

int main() {
    std::cout << "main test planning" << std::endl;

    const float MAX_DIVE_PPO2 = 1.4f;
    const float MAX_DECO_PPO2 = 1.6f;

    DiverParameters dp(MAX_DIVE_PPO2, MAX_DECO_PPO2, 60.0f, 20.0f);
    buhlmann::planning::WorkPlan wp = buhlmann::planning::WorkPlan(dp);
    wp.loadFromFile("../data/dp1.txt");
    wp.loadGassesFromFile("../data/gs1.txt");
    wp.contextualize();
    std::cout << wp << std::endl;

    Model bm(BuhlmannModelVersion::ZHL_16C, dp);
    bm.initialize();
    bm.clearModel();

    bm.runForWorkPlan(wp);

    std::cout << wp << std::endl;

    bm.clearModel();
    bm.runExplicitLogForWorkPlan(wp);
}
