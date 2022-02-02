/*
 * uart.hpp
 *
 *  Created on: 2019/03/29
 *      Author: User
 */

#ifndef MYHEADERS_UART_HPP_
#define MYHEADERS_UART_HPP_

#include <vector>
#include <array>
#include <string>
#include <functional>
#include <map>
#include <queue>
#include "stm32f3xx_hal.h"
#include "interrupt.hpp"

class UART
{
public:
	UART() = delete;
	UART(UART_HandleTypeDef* huart, InterruptAccessor::Map uart_it_map) : huart_(huart), uart_it_(uart_it_map, [&](){ receiveDataIT(); }){}

	inline void transmitData(uint8_t sendByte){ HAL_UART_Transmit(huart_, &sendByte, 1, 1000); }

	/*overloaded functions of transmitting.*/
	inline void transmitData(const char sendByte) { transmitData(static_cast<uint8_t>(sendByte)); }

	inline void transmitData(const uint8_t* pSendData, const size_t size) { for (size_t i = 0; i < size; i++)transmitData(*(pSendData + i)); }
	inline void transmitData(const char* pSendData, const size_t size) { for (size_t i = 0; i < size; i++)transmitData(*(pSendData + i)); }

	inline void transmitData(const std::string & sendString) { transmitData(sendString.c_str(), sendString.size()); }
	template<size_t S>
	inline void transmitData(const std::array<uint8_t, S> & sendDataArray) { for (auto i : sendDataArray)transmitData(static_cast<uint8_t>(i)); }
	template<size_t S>
	inline void transmitData(const std::array<char, S> & sendDataArray) { for (auto i : sendDataArray)transmitData(static_cast<uint8_t>(i)); }
	inline void transmitData(const std::vector<uint8_t> & sendVector) { for (auto i : sendVector)transmitData(i); }

	inline bool isDataReceived(){ return  huart_->RxState == HAL_UART_STATE_BUSY; }
	inline uint8_t receiveData(uint32_t timeout = UINT32_MAX)
	{
		uint8_t receiced_val;
		HAL_UART_Receive(huart_, &receiced_val, 1, timeout);
		return receiced_val;
	}
	inline void receiveDataIT()
	{
		receive_data_queue_.push(received_data_);
		HAL_UART_Receive_IT(huart_, &received_data_, 1);
	}
	inline auto readReceivedData(){ return received_data_; }
	inline uint8_t readReceiveDataQueue()
	{
		uint8_t val = receive_data_queue_.front();
		receive_data_queue_.pop();
		return val;
	}
	inline bool isReceiveDataQueueEmpty(){ return receive_data_queue_.empty(); }
private:
	UART_HandleTypeDef* huart_;
	uint8_t received_data_ = 0;
	InterruptAccessor uart_it_;
	std::queue<uint8_t> receive_data_queue_;
};
#endif /* MYHEADERS_UART_HPP_ */
