#include "simulation.h"

using namespace::std;

simulation::simulation(size_t bodies_count)
{
	m_bodies_count = bodies_count;
	m_bodies = new body[bodies_count];
	m_simulationTime = 0.0;
	m_deltaTime = 0.0;
}

void simulation::run(double delta_time, double simulation_time)
{
	m_deltaTime = delta_time;
	m_simulationTime = simulation_time;
	init();
	simulate();
}

void simulation::init()
{
	ANNOTATE_SITE_BEGIN(GENERATION)

	for (size_t i = 0; i < m_bodies_count; i++)
	{
		ANNOTATE_ITERATION_TASK(GENERATION)

		m_bodies[i].m_position = uniform_dist<double>(-0.5, 0.5);
		m_bodies[i].m_velocity = uniform_dist<double>(-0.5, 0.5);
		m_bodies[i].m_acceleration = { 0.0, 0.0, 0.0 };
		m_bodies[i].m_mass = random(1.0, 5.0);
	}

	ANNOTATE_SITE_END(GENERATION)

	cout << "Init simulation for " << m_bodies_count << " bodies" << endl;
	cout << "Total system energy: " << compute_full_energy() << " J" << endl;
}

void simulation::simulate()
{
	size_t iterationCount = (size_t)(m_simulationTime / m_deltaTime) + 1;
	alignas(ALIGN) double3* __restrict accelerations = new double3[m_bodies_count];

	for (size_t iteration = 1; iteration < iterationCount; iteration++)
	{
		auto iteration_start = clock();

		ANNOTATE_SITE_BEGIN(VELOCITY_AND_POSITION)

		for (size_t i = 0; i < m_bodies_count; i++)
		{
			ANNOTATE_ITERATION_TASK(VELOCITY_AND_POSITION)
			m_bodies[i].update_position_and_velocity(m_deltaTime);
		}

		ANNOTATE_SITE_END(VELOCITY_AND_POSITION)

		ANNOTATE_SITE_BEGIN(ACCELERATION)

		for (size_t my = 0; my < m_bodies_count; my++)
		{
			ANNOTATE_ITERATION_TASK(ACCELERATION)

			for (size_t other = 0; other < m_bodies_count; other++)
			{
				if (my == other)
					continue;

				auto direction = (m_bodies[my].m_position - m_bodies[other].m_position);
				auto inverse_koeff = pow(direction.lenght(), 3) + EPSILON;
				auto force_koeff = m_bodies[my].m_mass * G / inverse_koeff;
				accelerations[my] = m_bodies[my].m_acceleration + direction * force_koeff;
			}
		}

		ANNOTATE_SITE_END(ACCELERATION)

		for (size_t i = 0; i < m_bodies_count; i++)
		{
			m_bodies[i].m_acceleration = accelerations[i];
		}

		auto iteration_time = clock() - iteration_start;

		cout << "Step " << iteration << ". Total energy: " << compute_full_energy() << " J. Iteration time: " << iteration_time << " ms" << endl;
	}

	delete[] accelerations;
}

double simulation::compute_full_energy() const
{
	double potential_energy = 0.0;
	double kinetic_energy = 0.0;

	ANNOTATE_SITE_BEGIN(ENERGY)

	for (size_t i = 0; i < m_bodies_count - 1; i++)
	{
		ANNOTATE_ITERATION_TASK(ENERGY)

#pragma omp simd reduction(+:potential_energy)
#pragma vector aligned
#pragma vector always
		for (size_t j = i + 1; j < m_bodies_count; j++)
		{
			auto force_koeff = m_bodies[i].m_mass * m_bodies[j].m_mass * G;
			auto distance_koeff = (m_bodies[i].m_position - m_bodies[j].m_position).lenght() + EPSILON;
			potential_energy += force_koeff / distance_koeff;
		}
	}

	ANNOTATE_SITE_END(ENERGY)

#pragma unroll
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		kinetic_energy += m_bodies[i].m_mass * pow(m_bodies[i].m_velocity.lenght(), 2);
	}

	return potential_energy + 0.5 * kinetic_energy;
}
