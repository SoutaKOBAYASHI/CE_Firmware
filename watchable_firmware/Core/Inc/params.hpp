/*
 * params.hpp
 *
 *  Created on: Feb 1, 2022
 *      Author: KobayashiSouta
 */

#ifndef INC_PARAMS_HPP_
#define INC_PARAMS_HPP_

#include <utility>

constexpr uint64_t COUNT_G_TO_Y = 5*1000;
constexpr uint64_t COUNT_Y_TO_R = 3*1000;

constexpr std::pair<uint16_t, uint16_t> G_LED_VAL = {255, 0};
constexpr std::pair<uint16_t, uint16_t> Y_LED_VAL = {255, 255};
constexpr std::pair<uint16_t, uint16_t> R_LED_VAL = {0, 255};

#endif /* INC_PARAMS_HPP_ */
