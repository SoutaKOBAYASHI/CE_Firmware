/*
 * rplider.hpp
 *
 *  Created on: 2022/01/25
 *      Author: KobayashiSouta
 */

#ifndef INC_RPLIDER_H_
#define INC_RPLIDER_H_

#include <vector>
#include <utility>
#include <array>
#include "stm32f3xx_hal.h"
#include "vector2d.hpp"
#include "uart.hpp"
#include "interrupt.hpp"

class RPLIDER
{
public:
	RPLIDER()=delete;
	RPLIDER(UART& huart, InterruptAccessor::Map& systick_it_map):
		uart_(huart),
		systick_accessor_(systick_it_map, [&](){systick_call_func();})
	{}

	enum class command : uint8_t
	{
		STOP		= 0x25,
		RESET		= 0x40,
		SCAN		= 0x20,
		GET_INFO	= 0x50,
		GET_HEALTH	= 0x52
	};

	inline void transmitCmd(uint8_t cmd, const std::vector<uint8_t>& payload)
	{
		std::vector<uint8_t> transmit_vec = {TX_START_FLAG_, cmd};
		transmit_vec.insert(transmit_vec.end(), payload.begin(), payload.end());
		uart_.transmitData(transmit_vec);
	}
	inline void transmitCmd(uint8_t cmd)
	{
		std::vector<uint8_t> transmit_vec = {TX_START_FLAG_, cmd};
		uart_.transmitData(transmit_vec);
	}
	inline void transmitCmd(command cmd, const std::vector<uint8_t>& payload){ transmitCmd(static_cast<uint8_t>(cmd), payload); }
	inline void transmitCmd(command cmd){ transmitCmd(static_cast<uint8_t>(cmd)); }
	inline auto receiveDataIT(){ return uart_.receiveDataIT(); }
	inline bool isGotPointQueueEnpty() { return got_points_.empty(); }
	inline auto readGotPointQueue()
	{
		vector2d<double> data = got_points_.front();
		got_points_.pop();
		return data;
	}
	inline bool isScanning() const { return is_scan_started_; }
private:
	UART& uart_;
	InterruptAccessor systick_accessor_;
	static constexpr uint8_t TX_START_FLAG_ = 0xA5;
	static constexpr std::pair<uint8_t, uint8_t> RX_START_FLAG_ = {0xA5,0x5A};
	static constexpr size_t SCAN_DATA_SIZE_ = 5;
	static constexpr size_t START_SCAN_DATA_SIZE_ = 7;
	static constexpr std::array<uint8_t, START_SCAN_DATA_SIZE_> START_RESPONSE_DATA_ = {0xA5, 0x5A, 0x05, 0x00, 0x00, 0x40, 0x81};
	std::queue<vector2d<double>> got_points_;
	bool is_scan_started_ = false;
	bool is_needed_retransmit_ = false;
	static constexpr uint8_t RETRANSMIT_INTERVAL = 5;
	inline void receiveData_(uint8_t receive_data)
	{
		if(is_scan_started_)
		{
			receiveMeasureData_(receive_data);
		}
		else
		{
			receiveResponseData_(receive_data);
		}
	}
	void systick_call_func()
	{
		while(!uart_.isReceiveDataQueueEmpty()) receiveData_(uart_.readReceiveDataQueue());
		retransmitScanSig();
	}
	void receiveResponseData_(uint8_t receive_data)
	{
		static uint8_t count = 0;
		if(START_RESPONSE_DATA_.at(count) == receive_data)
		{
			++count;
			if(count == START_SCAN_DATA_SIZE_)
			{
				is_scan_started_ = true;
				count = 0;
			}
			is_needed_retransmit_ = false;
		}
		else
		{
			count = 0;
			is_needed_retransmit_ = true;
		}
		uart_.receiveDataIT();
	}
	void receiveMeasureData_(uint8_t receive_data)
	{
		uart_.receiveDataIT();

		constexpr double DIST_GAIN = 1/4.0;
		constexpr double ANGLE_GAIN = -1.0 * (M_PI/180)*(1/64.0);

		uint16_t angle = 0, dist = 0;
		double angle_act = 0, dist_act = 0;
		static std::array<uint8_t, 5> received_data_arr = {};
		static uint8_t count = 0;

		received_data_arr.at(count) = uart_.readReceivedData();
		++count;
		if(count == 5)
		{
			angle 	= static_cast<uint16_t>(received_data_arr.at(2)) << 7 | static_cast<uint16_t>(received_data_arr.at(1) & 0xFE) >> 1;
			dist	= static_cast<uint16_t>(received_data_arr.at(4)) << 8 | static_cast<uint16_t>(received_data_arr.at(3));
			angle_act = static_cast<double>(angle) * ANGLE_GAIN;
			dist_act = static_cast<double>(dist) * DIST_GAIN;
			got_points_.push(make_vector2d_pol(dist_act, angle_act));

			count = 0;
		}
	}
	void retransmitScanSig()
	{
		static uint8_t count = 0;
		static bool is_resetted = false;
		if(count < RETRANSMIT_INTERVAL)
		{
			++count;
		}
		else
		{
			if(is_needed_retransmit_)
			{
				is_needed_retransmit_ = false;
				is_resetted = true;
				transmitCmd(command::STOP);
			}
			if(is_resetted)
			{
				if(count < 2*RETRANSMIT_INTERVAL)
				{
					++count;
				}
				else
				{
					is_resetted = false;
					count = 0;
					transmitCmd(command::SCAN);
				}
			}
		}
	}
};

#endif /* INC_RPLIDER_H_ */
