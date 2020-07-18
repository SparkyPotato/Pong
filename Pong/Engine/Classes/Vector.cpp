#include "Vector.h"

Vector::Vector()
	:
	x(0),
	y(0)
{}

Vector::Vector(float x, float y)
	:
	x(x),
	y(y)
{}

Vector::Vector(const Vector& copy)
	:
	x(copy.x),
	y(copy.y)
{}

Vector Vector::operator+(const Vector & other)
{
	return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector& other)
{
	return Vector(x - other.x, y - other.y);
}

Vector Vector::operator*(const float& scalar)
{
	return Vector(x * scalar, y * scalar);
}

float Vector::operator*(const Vector& dot)
{
	return x * dot.x + y * dot.y;
}

Vector Vector::operator/(const float& scalar)
{
	return Vector(x / scalar, y / scalar);
}

float Vector::GetLength()
{
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

Vector Vector::GetNormalized()
{
	float length = GetLength();
	return Vector(x / length, y / length);
}

Vector Vector::operator+=(const Vector& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector Vector::operator-=(const Vector& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}