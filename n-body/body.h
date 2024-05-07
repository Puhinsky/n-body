#pragma once

#include "vector3.h"
#include <vector>

constexpr auto F1_KOEFF = 2.0 / 3.0;
constexpr auto F2_KOEFF = 1.0 / 3.0;
constexpr auto KR_KOEFF = 1.0 / 4.0;

class body
{
private:
	double3 compute(double dt, double3 diff);

public:
	double3 m_position;
	double3 m_velocity;
	double3 m_acceleration;
	double m_mass;

	void update_position_and_velocity(double deltaTime);

	inline double compute_2k_energy() const
	{
		return m_mass * pow(m_velocity.lenght(), 2);
	}
	
	inline double3 compute_impulse() const
	{
		return m_velocity * m_mass;
	}
};
