//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_UNITS_HPP
#define LIBBUHLMAN_UNITS_HPP

constexpr float FT_H2O_PER_ATM = 33.0f;
constexpr float ATM_PER_FT_H20 = 1 / FT_H2O_PER_ATM;
constexpr float MINIMUM_SAFE_PPO2 = 0.18f;

constexpr float SECONDS_PER_MINUTE = 60.0f;

#include "Depth.hpp"
#include "DepthRate.hpp"
#include "Pressure.hpp"
#include "PressureRate.hpp"
#include "Temperature.hpp"
#include "Time.hpp"

#endif //LIBBUHLMAN_UNITS_HPP
