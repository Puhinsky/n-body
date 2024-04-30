#pragma once

#include <ostream>

template<typename T>
struct vector3
{
public:
	T x, y, z;

	vector3 operator*(const T& other) const;
	vector3 operator+(const vector3& other) const;
	vector3 operator-(const vector3& other) const;
	vector3 operator/(const T& other)const;
	void operator+=(const vector3& other);

	void print(std::ostream& os);

	vector3 normalize() const;
	T lenght() const;
};

typedef vector3<double> double3;

template<typename T>
inline vector3<T> vector3<T>::operator*(const T& other) const
{
	return { x * other, y * other, z * other };
}

template<typename T>
inline vector3<T> vector3<T>::operator+(const vector3& other) const
{
	return {x + other.x, y + other.y, z + other.z};
}

template<typename T>
inline vector3<T> vector3<T>::operator-(const vector3& other) const
{
	return {x - other.x, y - other.y, z - other.z};
}

template<typename T>
inline vector3<T> vector3<T>::operator/(const T& other) const
{
	return { x / other, y / other, z / other };
}

template<typename T>
inline void vector3<T>::operator+=(const vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

template<typename T>
inline void vector3<T>::print(std::ostream& os)
{
	os << x << '\t' << y << '\t' << z << std::endl;
}

template<typename T>
inline vector3<T> vector3<T>::normalize() const
{
	return *this / this->lenght();
}

template<typename T>
inline T vector3<T>::lenght() const
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
