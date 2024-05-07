#pragma once

#include "vector3.h"
#include <vector>

constexpr auto F1_KOEFF = 2.0 / 3.0;
constexpr auto F2_KOEFF = 1.0 / 3.0;
constexpr auto KR_KOEFF = 1.0 / 4.0;

class body
{
private:
	double3 m_position_f[3];
	double3 m_velocity_f[3];

	double3 m_base_position;
	double3 m_base_velocity;
	
	inline double3 compute_f1(double dt, double3 diff)
	{
		return diff * dt;
	}

	inline double3 compute_f2(double dt, double3 diff, double3 f1)
	{
		return (diff + f1 * F1_KOEFF) * dt;
	}

	inline double3 compute(double dt, double3 diff, double3 f1, double3 f2)
	{
		auto f3 = (diff - f1 * F2_KOEFF + f2) * dt;

		return f1 + f2 * 2.0 + f3;
	}

public:
	double3 m_position;
	double3 m_velocity;
	double3 m_acceleration;
	double m_mass;

	void update_position_and_velocity(double delta_time);
	void pre_compute_f1(double delta_time);
	void pre_compute_f2(double delta_time);

	inline double compute_2k_energy() const
	{
		return m_mass * pow(m_velocity.lenght(), 2);
	}
	
	inline double3 compute_impulse() const
	{
		return m_velocity * m_mass;
	}
};
