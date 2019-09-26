#include "curve.h"


void Curve::drawPoints()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < num_vertices; i++)
	{
		glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
	}
	glEnd();
}

void Curve::drawLine()
{
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < num_vertices; i++)
	{
		glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
		
	}
	glEnd();

}

