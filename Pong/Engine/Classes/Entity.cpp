#include "Entity.h"

Shape::Shape(Vector* vectorArray, int verticeCount)
{
	vertexCount = verticeCount;
	vertices = vectorArray;
}

Shape::Shape(const Shape& other)
{
	vertexCount = other.vertexCount;
	vertices = new Vector[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i] = other.vertices[i];
	}
}

Shape::Shape(Shape&& other)
{
	vertexCount = other.vertexCount;
	vertices = other.vertices;
}

Entity::Entity(Shape shape, Vector position)
	:
	Object(position),
	shape(shape)
{}

Entity::Entity(const Entity& copy)
	:
	shape(copy.shape)
{
	position = copy.position;
}