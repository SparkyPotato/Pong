#pragma once
#include <Minimal.h>

class Object
{
public:
	Object() = default;
	Object(Vector position);

	Vector GetPosition() { return position; }
	void SetPosition(Vector newPos) { position = newPos; }

	Vector GetVelocity() { return velocity; }
	void SetVelocity(Vector newVelocity) { velocity = newVelocity; }
	
protected:
	Vector position;
	Vector velocity = Vector(0.f, 0.f);
};

