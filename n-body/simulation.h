#pragma once

#include "body.h"
#include "random.h"
#include "vector3.h"
#include <iostream>

constexpr auto G = 6.67259e-11;
constexpr auto EPSILON = 1e-6;
constexpr auto ALIGN = 64;

class simulation
{
private:
	alignas(ALIGN) body* __restrict m_bodies;
	alignas(ALIGN) double* __restrict m_potential_energies;
	size_t m_bodies_count;
	double m_delta_time;
	double m_simulationTime;
	double m_init_energy;

	void init();
	void simulate();
	void compute_accelerations();
	double compute_full_energy() const;
	double compute_impulse() const;

public:
	simulation(size_t bodies_count);
	void run(double delta_time, double simulation_time);
};
