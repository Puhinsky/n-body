#pragma once

#include "vector3.h"
#include <random>

template<typename T>
vector3<T> uniform_dist(T from, T to);

template<typename T, typename Generator = std::mt19937>
T random(T from, T to);

template<typename T>
inline vector3<T> uniform_dist(T from, T to)
{
	return { random(from, to), random(from, to), random(from, to) };
}

template<typename T, typename Generator>
inline T random(T from, T to)
{
	thread_local static Generator gen(std::random_device{}());

	using dist_type = typename std::conditional
		<
		std::is_integral<T>::value,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>
		>::type;

	thread_local static dist_type dist;

	return dist(gen, typename dist_type::param_type{ from, to });
}
