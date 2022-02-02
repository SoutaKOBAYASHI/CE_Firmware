/*
 * interrupt.hpp
 *
 *  Created on: 2022/01/27
 *      Author: KobayashiSouta
 */

#ifndef INC_INTERRUPT_HPP_
#define INC_INTERRUPT_HPP_

#include <functional>
#include <map>


class InterruptAccessor
{
public:
	using Map = std::map<InterruptAccessor* const,const std::function<void(void)>>;
	InterruptAccessor(Map& it_func_map, const std::function<void(void)>& func) :
		it_func_map_(it_func_map) { it_func_map_.insert(std::make_pair(this, func)); }
	virtual ~InterruptAccessor(){ it_func_map_.erase(this); }
private:
	Map& it_func_map_;
};

#endif /* INC_INTERRUPT_HPP_ */
