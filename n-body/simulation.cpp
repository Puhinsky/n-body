#include "simulation.h"

using namespace::std;

simulation::simulation(size_t bodies_count)
{
	m_bodies_count = bodies_count;
	m_bodies = new body[bodies_count];
	m_simulationTime = 0.0;
	m_delta_time = 0.0;
}

void simulation::run(double delta_time, double simulation_time)
{
	m_delta_time = delta_time;
	m_simulationTime = simulation_time;
	m_potential_energies = new double[m_bodies_count];
	init();
	simulate();

	delete[] m_bodies;
	delete[] m_potential_energies;
}

void simulation::init()
{

#pragma unroll
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		m_bodies[i].m_position = uniform_dist<double>(-0.5, 0.5);
		m_bodies[i].m_velocity = uniform_dist<double>(-0.5, 0.5);
		m_bodies[i].m_acceleration = { 0.0, 0.0, 0.0 };
		m_bodies[i].m_mass = random(1.0, 5.0);
	}

	m_init_energy = compute_full_energy();

	cout << "Init simulation for " << m_bodies_count << " bodies" << endl;
	cout << "Total system energy: " << m_init_energy << " J" << endl;
	cout << "System impulse: " << compute_impulse() << " kg*m/s" << endl;
}

void simulation::simulate()
{
	size_t iterationCount = (size_t)(m_simulationTime / m_delta_time) + 1;
	double energy = 0.0;

	for (size_t iteration = 1; iteration < iterationCount; iteration++)
	{
		auto iteration_start = clock();
		auto fracted_delta_time = m_delta_time / 3.0;

#pragma omp simd
#pragma vector aligned
#pragma vector always
		for (size_t i = 0; i < m_bodies_count; i++)
		{
			m_bodies[i].pre_compute_f1(fracted_delta_time);
		}

		compute_accelerations();

#pragma omp simd
#pragma vector aligned
#pragma vector always
		for (size_t i = 0; i < m_bodies_count; i++)
		{
			m_bodies[i].pre_compute_f2(fracted_delta_time);
		}

		compute_accelerations();

#pragma omp simd
#pragma vector aligned
#pragma vector always
		for (size_t i = 0; i < m_bodies_count; i++)
		{
			m_bodies[i].update_position_and_velocity(fracted_delta_time);
		}

		energy = compute_full_energy();
		auto iteration_time = clock() - iteration_start;

		cout << "Step " << iteration << ". Total energy: " << energy << " J. Iteration time: " << iteration_time << " ms" << endl;
	}

	auto absolute_deviation = abs(energy - m_init_energy);
	auto relative_deviation = absolute_deviation / energy;

	cout << "Energy deviation: " << relative_deviation * 100 << " %" << endl;
	cout << "Final impulse: " << compute_impulse() << " kg*m/s" << endl;
}

void simulation::compute_accelerations()
{
#pragma omp simd
#pragma vector aligned
#pragma vector always
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		m_bodies[i].m_acceleration = { 0.0, 0.0, 0.0 };
	}

#pragma omp parallel for
	for (size_t my = 0; my < m_bodies_count; my++)
	{
		for (size_t other = 0; other < m_bodies_count; other++)
		{
			if (my == other)
				continue;

			auto direction = (m_bodies[my].m_position - m_bodies[other].m_position);
			auto inverse_koeff = pow(direction.lenght(), 3) + EPSILON;
			auto force_koeff = m_bodies[my].m_mass * G / inverse_koeff;
			m_bodies[my].m_acceleration += direction * force_koeff;
		}
	}
}

double simulation::compute_full_energy() const
{
	double potential_energy = 0.0;
	double kinetic_energy = 0.0;

#pragma omp simd
#pragma vector always
#pragma vector aligned
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		m_potential_energies[i] = 0.0;
	}

#pragma omp parallel for
	for (size_t i = 0; i < m_bodies_count - 1; i++)
	{
#pragma omp simd reduction(+:potential_energy)
#pragma vector aligned
#pragma vector always
		for (size_t j = i + 1; j < m_bodies_count; j++)
		{
			auto force_koeff = m_bodies[i].m_mass * m_bodies[j].m_mass * G;
			auto distance_koeff = (m_bodies[i].m_position - m_bodies[j].m_position).lenght() + EPSILON;
			m_potential_energies[i] += force_koeff / distance_koeff;
		}
	}

#pragma omp simd reduction(+:potential_energy)
#pragma vector always
#pragma vector aligned
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		potential_energy += m_potential_energies[i];
	}

#pragma unroll
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		kinetic_energy += m_bodies[i].compute_2k_energy();
	}

	return potential_energy + 0.5 * kinetic_energy;
}

double simulation::compute_impulse() const
{
	double3 impulse = { 0.0, 0.0, 0.0 };

#pragma unroll
	for (size_t i = 0; i < m_bodies_count; i++)
	{
		impulse += m_bodies[i].compute_impulse();
	}

	return impulse.lenght();
}
