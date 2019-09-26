#include "surfaceOfRevolution.h"

SurfaceOfRevolution::SurfaceOfRevolution(Curve* c)
{
	curve = c;
	num_vertices = curve->getNumVertices();
	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = curve->getVertex(i);
	}
}

SurfaceOfRevolution::~SurfaceOfRevolution()
{
	delete curve;
}

void SurfaceOfRevolution::drawCurve(ArgParser* args)
{
	curve_tessellation = args->curve_tessellation;
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	if (curve->getTypeId() == 1) {
		for (int i = 0; i < num_vertices - 3; i += 3)
		{
			for (int j = 0; j <= curve_tessellation; j++)
			{
				//GLfloat x = vertices[i].x() * pow(1.0f - t, 3) + 3 * vertices[1 + i].x() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].x() * t * t * (1.0f - t) + vertices[3 + i].x() * pow(t, 3);
				//GLfloat y = vertices[i].y() * pow(1.0f - t, 3) + 3 * vertices[1 + i].y() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].y() * t * t * (1.0f - t) + vertices[3 + i].y() * pow(t, 3);
				//GLfloat z = vertices[i].z() * pow(1.0f - t, 3) + 3 * vertices[1 + i].z() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].z() * t * t * (1.0f - t) + vertices[3 + i].z() * pow(t, 3);

				float t = 1.0f * j / curve_tessellation;

				Vec3f p01 = vertices[i] * (1 - t) + vertices[i + 1] * t;
				Vec3f p11 = vertices[i + 1] * (1 - t) + vertices[i + 2] * t;
				Vec3f p21 = vertices[i + 2] * (1 - t) + vertices[i + 3] * t;
				Vec3f p02 = p01 * (1 - t) + p11 * t;
				Vec3f p12 = p11 * (1 - t) + p21 * t;
				Vec3f p03 = p02 * (1 - t) + p12 * t;

				glVertex3f(p03.x(), p03.y(), p03.z());

			}
		}
	}
	else if (curve->getTypeId() == 2) {
		for (int i = 0; i < num_vertices - 3; i++)
		{
			for (int j = 0; j <= curve_tessellation; j++)
			{
				float t = 1.0 * j / curve_tessellation;
				float x = vertices[i].x() * pow(1.0f - t, 3) / 6 + vertices[1 + i].x() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].x() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].x() * pow(t, 3) / 6;
				float y = vertices[i].y() * pow(1.0f - t, 3) / 6 + vertices[1 + i].y() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].y() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].y() * pow(t, 3) / 6;
				float z = vertices[i].z() * pow(1.0f - t, 3) / 6 + vertices[1 + i].z() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].z() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].z() * pow(t, 3) / 6;

				//是否可以优化为递归计算
				glVertex3f(x, y, z);

			
			}
		}
	}
	glEnd();
}

void SurfaceOfRevolution::Paint(ArgParser* args)
{
	drawLine();
	drawPoints();
	drawCurve(args);
}

TriangleMesh* SurfaceOfRevolution::OutputTriangles(ArgParser* args)
{
	return curve->OutputTriangles(args);
}

void SurfaceOfRevolution::OutputBSpline(FILE* file)
{
	fprintf(file, "bspline\n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		fprintf(file, "%f ", vertices[i].x());
		fprintf(file, "%f ", vertices[i].y());
		fprintf(file, "%f\n", vertices[i].z());
	}
}

