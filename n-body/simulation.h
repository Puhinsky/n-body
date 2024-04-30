#pragma once

#include <iostream>
#include <vector>
#include "body.h"
#include "vector3.h"
#include "random.h"

constexpr auto G = 6.67259e-11;
constexpr auto EPSILON = 1e-6;

class simulation
{
private:
	std::vector<body> m_bodies;
	double m_deltaTime;
	double m_simulationTime;

	void init();
	void simulate();
	double compute_full_energy() const;

public:
	simulation(size_t bodiesCount);
	void run(double deltaTime, double simulationTime);
};
