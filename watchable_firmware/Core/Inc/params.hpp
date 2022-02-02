/*
 * params.hpp
 *
 *  Created on: Feb 1, 2022
 *      Author: KobayashiSouta
 */

#ifndef INC_PARAMS_HPP_
#define INC_PARAMS_HPP_

#include <utility>

constexpr uint64_t COUNT_G_TO_Y = 5*1000*60;
constexpr uint64_t COUNT_G_TO_R = 5*1000*60;

constexpr std::pair<uint16_t, uint16_t> G_LED_VAL = {80, 0};
constexpr std::pair<uint16_t, uint16_t> Y_LED_VAL = {20, 20};
constexpr std::pair<uint16_t, uint16_t> R_LED_VAL = {0, 20};

#endif /* INC_PARAMS_HPP_ */
