#include "body.h"

void body::pre_compute_f1(double delta_time)
{
	m_base_position = m_position;
	m_base_velocity = m_velocity;

	m_position_f[0] = compute_f1(delta_time, m_velocity);
	m_velocity_f[0] = compute_f1(delta_time, m_acceleration);

	m_position += m_position_f[0];
	m_velocity += m_velocity_f[0];
}

void body::pre_compute_f2(double delta_time)
{
	m_position_f[1] = compute_f2(delta_time, m_velocity, m_position_f[0]);
	m_velocity_f[1] = compute_f2(delta_time, m_acceleration, m_velocity_f[0]);

	m_position += m_position_f[1];
	m_velocity += m_velocity_f[1];
}

void body::update_position_and_velocity(double delta_time)
{
	auto delta_position = compute(delta_time, m_velocity, m_position_f[0], m_position_f[1]) * KR_KOEFF;
	auto delta_velocity = compute(delta_time, m_acceleration, m_velocity_f[0], m_velocity_f[1]) * KR_KOEFF;

	m_position = m_base_position + delta_position;
	m_velocity = m_base_velocity + delta_velocity;
}