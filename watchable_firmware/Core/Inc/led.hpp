/*
 * led.hpp
 *
 *  Created on: Feb 1, 2022
 *      Author: KobayashiSouta
 */

#ifndef INC_LED_HPP_
#define INC_LED_HPP_

#include "stm32f3xx_hal.h"
#include "params.hpp"

class LED
{
public:
	LED()=delete;
	LED(TIM_HandleTypeDef* htim) : timer_(htim) {}

	enum class Color{G = 0, Y = 1, R = 2};

	void setColor(Color color)
	{
		switch (color) {
			case Color::G:
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_1, G_LED_VAL.first);
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_2, G_LED_VAL.second);
				break;
			case Color::Y:
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_1, Y_LED_VAL.first);
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_2, Y_LED_VAL.second);
				break;
			case Color::R:
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_1, R_LED_VAL.first);
				__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_2, R_LED_VAL.second);
				break;
			default:
				break;
		}

		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_2);
	}
	void setColor(std::pair<uint16_t, uint16_t>& color)
	{
		__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_1, color.first);
		__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_2, color.second);
		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_2);
	}
	void setColor(uint16_t red_param, uint16_t green_param)
	{
		__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_1, green_param);
		__HAL_TIM_SET_COMPARE(timer_, TIM_CHANNEL_2, red_param);
		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(timer_, TIM_CHANNEL_2);
	}
private:
	TIM_HandleTypeDef* timer_;
};

#endif /* INC_LED_HPP_ */
