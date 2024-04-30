#include "simulation.h"

using namespace::std;

simulation::simulation(size_t bodiesCount)
{
	m_bodies = vector<body>(bodiesCount);
	m_simulationTime = 0.0;
	m_deltaTime = 0.0;
}

void simulation::run(double deltaTime, double simulationTime)
{
	m_deltaTime = deltaTime;
	m_simulationTime = simulationTime;
	init();
	simulate();
}

void simulation::init()
{
	for (auto& body : m_bodies)
	{
		body.m_position = uniform_dist<double>(-0.5, 0.5);
		body.m_velocity = uniform_dist<double>(-0.5, 0.5);
		body.m_acceleration = { 0.0, 0.0, 0.0 };
		body.m_mass = random(1.0, 5.0);
	}

	cout << "Init simulation for " << m_bodies.size() << " bodies" << endl;
	cout << "Total system energy: " << compute_full_energy() << " J" << endl;
}

void simulation::simulate()
{
	size_t iterationCount = (size_t)(m_simulationTime / m_deltaTime) + 1;
	vector<double3> accelerations(m_bodies.size());

	for (size_t iteration = 1; iteration < iterationCount; iteration++)
	{
		auto iteration_start = clock();

		for (auto& body : m_bodies)
		{
			body.update_position_and_velocity(m_deltaTime);
		}

		for (size_t my = 0; my < m_bodies.size(); my++)
		{
			for (size_t other = 0; other < m_bodies.size(); other++)
			{
				if (my == other)
					continue;

				auto direction = (m_bodies[my].m_position - m_bodies[other].m_position);
				auto inverse_koeff = pow(direction.lenght(), 3) + EPSILON;
				auto force_koeff = m_bodies[my].m_mass * G / inverse_koeff;
				accelerations[my] += direction * force_koeff;
			}
		}

		for (size_t i = 0; i < m_bodies.size(); i++)
		{
			m_bodies[i].m_acceleration = accelerations[i];
		}

		auto iteration_time = clock() - iteration_start;

		cout << "Step " << iteration << ". Total energy: " << compute_full_energy() << " J. Iteration time: " << iteration_time << " ms" << endl;
	}
}

double simulation::compute_full_energy() const
{
	double potential_energy = 0.0;
	double kinetic_energy = 0.0;

	for (size_t i = 0; i < m_bodies.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_bodies.size(); j++)
		{
			auto force_koeff = m_bodies[i].m_mass * m_bodies[j].m_mass * G;
			auto distance_koeff = (m_bodies[i].m_position - m_bodies[j].m_position).lenght() + EPSILON;
			potential_energy += force_koeff / distance_koeff;
		}
	}

	for (size_t i = 0; i < m_bodies.size() - 1; i++)
	{
		kinetic_energy += m_bodies[i].m_mass * pow(m_bodies[i].m_velocity.lenght(), 2);
	}

	return potential_energy + 0.5 * kinetic_energy;
}
