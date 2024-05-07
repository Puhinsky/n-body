#pragma once

#include "body.h"
#include "random.h"
#include "vector3.h"
#include <iostream>

constexpr auto G = 6.67259e-11;
constexpr auto EPSILON = 1e-6;

class simulation
{
private:
	body* m_bodies;
	size_t m_bodies_count;
	double m_deltaTime;
	double m_simulationTime;
	double m_init_energy;

	void init();
	void simulate();
	double compute_full_energy() const;
	double compute_impulse() const;

public:
	simulation(size_t bodies_count);
	void run(double delta_time, double simulation_time);
};
