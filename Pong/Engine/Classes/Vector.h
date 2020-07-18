#pragma once
#include <Minimal.h>

class Vector
{
public:
	Vector();
	Vector(float x, float y);
	Vector(const Vector& copy);

	Vector operator+ (const Vector& other);
	Vector operator- (const Vector& other);
	Vector operator* (const float& scalar);
	float operator* (const Vector& dot);
	Vector operator/ (const float& scalar);
	Vector operator+= (const Vector& other);
	Vector operator-= (const Vector& other);

	float GetLength();
	Vector GetNormalized();

	float x;
	float y;
};

