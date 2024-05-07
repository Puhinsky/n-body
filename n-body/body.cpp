#include "body.h"

double3 body::compute(double dt, double3 diff)
{
	auto f1 = diff * dt;
	auto f2 = (diff + f1 * F1_KOEFF) * dt;
	auto f3 = (diff - f1 * F2_KOEFF + f2) * dt;

	return f1 + f2 * 2.0 + f3;
}

void body::update_position_and_velocity(double deltaTime)
{
	m_position += compute(deltaTime, m_velocity) * KR_KOEFF;
	m_velocity += compute(deltaTime, m_acceleration) * KR_KOEFF;
}
