#pragma once
#include <cmath>

template<typename T>
class Vector3
{
public:
	//Set the default value of a vector to 0
	Vector3() : x(0), y(0), z(0) {}
	//Initialise vector with given values
	Vector3(T x, T y, T z) : x(x), y(y), z(z) {};
	//copy constructor for the vector
	Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
	//vector destructor
	~Vector3() {}

	//X and Y values with a template type
	T x, y, z;

	//Get the magnitude (Length) of a vector
	float Magnitude()
	{
		return std::sqrt((x * x) + (y * y) + (z * z));
	}
	//Normalise the vector using its own magnitude
	void Normalise()
	{
		x / Magnitude();
		y / Magnitude();
		z / Magnitude();
	}

	//Add a vector to the current vector
	Vector3& operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	//subtract a vector to the current vector
	Vector3& operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	//Set a vector value to the current vector
	Vector3& operator=(Vector3 rhs)
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
		std::swap(z, rhs.z);
		return *this;
	}

	//output the vector to OS stream
	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
	{
		return os << "(x=" << vec.x << " y=" << vec.y << "z=" << vec.z <<")";
	}

};

//Add two vectors together
template<typename T>
inline Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
//Subtract two vectors together
template<typename T>
inline Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

//Multiply a vector by a scalar value
template<typename T>
inline Vector3<T> operator*(Vector3<T> lhs, T rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

//Divide a vector by a scalar value
template<typename T>
inline Vector3<T> operator/(Vector3<T> lhs, T rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}

//Check if two vectors are equal
template<typename T>
inline bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z)
		return true;
	return false;
}
//Check if two vectors are NOT equal
template<typename T>
inline bool operator!=(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	if (lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z)
		return true;
	return false;
}
//Get the inverted value of a vector
template<typename T>
inline Vector3<T> operator-(Vector3<T>& rhs)
{
	return { -rhs.x,-rhs.y,-rhs.z };
}

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;