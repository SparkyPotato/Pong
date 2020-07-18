#pragma once
#include <Minimal.h>
#include "Object.h"

class Shape
{
public:
	Shape(Vector* verticeArray, int vertexCount);
	Shape(const Shape& other);
	Shape(Shape&& other);

	int GetVertexCount() { return vertexCount; }
	Vector* GetVertices() { return vertices; }

private:
	Vector* vertices;
	int vertexCount;
};

class Entity : public Object
{
public:
	Entity(Shape shape, Vector position);
	Entity(const Entity& copy);

	Shape GetShape() { return shape; }

private:
	Shape shape;
};