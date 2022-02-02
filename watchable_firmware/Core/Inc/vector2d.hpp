/*
 * vector2d.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */
#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include <initializer_list>
#include <utility>
#include <cmath>
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif


template<typename _Tp>
class vector2d
{
private:
	_Tp point_x_;
	_Tp point_y_;

public:
	constexpr vector2d() : point_x_(0), point_y_(0) { }
	constexpr vector2d(std::initializer_list<_Tp> init) : point_x_(*init.begin()), point_y_(*(init.begin()+1)) { }
	constexpr vector2d(const vector2d<_Tp>& copy)
		: point_x_(copy.rec().first), point_y_(copy.rec().second) { }

	constexpr vector2d<_Tp> operator+=(const vector2d<_Tp>& add_vector)
	{
		point_x_ += add_vector.rec().first;
		point_y_ += add_vector.rec().second;
		return  *this;
	}
	constexpr vector2d<_Tp> operator-=(const vector2d<_Tp>& subtract_vector)
	{
		point_x_ -= subtract_vector.rec().first;
		point_y_ -= subtract_vector.rec().second;
		return  *this;
	}
	constexpr vector2d<_Tp> operator+(const vector2d<_Tp>& add_vector) const
	{
		return {this->rec().first + add_vector.rec().first, this->rec().second + add_vector.rec().second};
	}
	constexpr vector2d<_Tp> operator-(const vector2d<_Tp>& subtract_vector) const
	{
		return {this->rec().first - subtract_vector.rec().first, this->rec().second - subtract_vector.rec().second};
	}
	constexpr _Tp operator*(const vector2d<_Tp>& add_vector) const
	{
		return (point_x_ * add_vector.rec().first) + (point_y_ * add_vector.rec().second);
	}
    constexpr vector2d<_Tp> operator=(const vector2d<_Tp>& substitute_vector)
    {
        point_x_ = substitute_vector.rec().first;
        point_y_ = substitute_vector.rec().second;
        return *this;
    }

	constexpr std::pair<_Tp, _Tp> rec() const { return std::make_pair(point_x_, point_y_); }
	constexpr std::pair<_Tp, _Tp> pol() const
	{
		_Tp rad = std::hypot(point_x_, point_y_);
		_Tp theta = std::atan2(point_y_, point_x_);
		return std::make_pair(rad, theta);
	}

	constexpr _Tp size() const { return pol().first; }
};

template<typename _Tp>
constexpr vector2d<_Tp> make_vector2d(_Tp point_x, _Tp point_y)
{
	vector2d<_Tp> made_vector = {point_x, point_y};
	return  made_vector;
}

template<typename _Tp>
constexpr vector2d<_Tp> make_vector2d_pol(_Tp radius, _Tp theta)
{
	vector2d<_Tp> made_vector = {radius * std::cos(theta), radius * std::sin(theta)};
	return  made_vector;
}

#endif
