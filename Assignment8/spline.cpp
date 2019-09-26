#include "spline.h"

void Spline::set(int i, Vec3f v)
{
	vertices[i] = v;
}

void Spline::moveControlPoint(int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set(x, y, vertices[selectedPoint].z());
}

void Spline::addControlPoint(int selectedPoint, float x, float y)
{
	num_vertices++;
	Vec3f* temp = new Vec3f[num_vertices];
	for (int i = 0; i < selectedPoint; i++)
	{
		temp[i] = vertices[i];
	}
	temp[selectedPoint].Set(x, y, 0);
	for (int i = selectedPoint + 1; i < num_vertices; i++)
	{
		temp[i] = vertices[i - 1];
	}
	delete[] vertices;
	vertices = new  Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}
}

void Spline::deleteControlPoint(int selectedPoint)
{
	if (num_vertices <= 4)
		return;
	for (int i = selectedPoint; i < num_vertices - 1; i++)
	{
		vertices[i] = vertices[i + 1];
	}
	num_vertices--;
}
