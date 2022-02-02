/*
 * switch.hpp
 *
 *  Created on: Feb 2, 2022
 *      Author: kobasou
 */

#ifndef INC_SWITCH_HPP_
#define INC_SWITCH_HPP_

#include "stm32f3xx_hal.h"
#include "interrupt.hpp"

class Switch
{
public:
	Switch() = delete;
	Switch(GPIO_TypeDef* gpio_port, uint16_t gpio_pin, InterruptAccessor::Map& exti_it_map) :
		gpio_port_(gpio_port), gpio_pin_(gpio_pin), exti_it_(exti_it_map, [&](){ is_read_ = true; }){}

	inline bool isRead()
	{
		bool current_val = is_read_;
		is_read_ = false;
		return current_val;
	}
private:
	GPIO_TypeDef* const gpio_port_;
	const uint16_t gpio_pin_;
	InterruptAccessor exti_it_;

	bool is_read_ = false;
};

#endif /* INC_SWITCH_HPP_ */
