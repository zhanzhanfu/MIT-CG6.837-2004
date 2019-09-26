#include "bSplineCurve.h"

BSplineCurve::BSplineCurve(int num_v)
{
	num_vertices = num_v;
	vertices = new Vec3f[num_vertices];
	type_id = 2;
}

BSplineCurve::~BSplineCurve()
{
	delete[] vertices;
}

void BSplineCurve::Paint(ArgParser* args)
{
	drawLine();
	drawPoints();
	drawCurve(args);
}

void BSplineCurve::OutputBezier(FILE* file)
{
	Matrix gbz, gbs;
	for (int i = 0; i < num_vertices; i++)
	{
		gbs.Set(i, 0, vertices[i].x());
		gbs.Set(i, 1, vertices[i].y());
		gbs.Set(i, 2, vertices[i].z());
	}
	float a_bbz[16] = { -1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0 };
	float a_bbs[16] = { -1,3,-3,1,3,-6,0,4,-3,3,3,1,1,0,0,0 };
	Matrix bbz(a_bbz);
	Matrix bbs(a_bbs);
	bbs = bbs * (1.0f / 6);
	bbz.Inverse();
	gbz = gbs * bbs * bbz;
	fprintf(file, "bezier \n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		fprintf(file, "%f ", gbz.Get(i, 0));
		fprintf(file, "%f ", gbz.Get(i, 1));
		fprintf(file, "%f\n", gbz.Get(i, 2));
	}
}

void BSplineCurve::OutputBSpline(FILE* file)
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

TriangleMesh* BSplineCurve::OutputTriangles(ArgParser* args)
{
	curve_tessellation = args->curve_tessellation;
	int revolution_tessellation = args->revolution_tessellation;
	float degree = 2 * M_PI / revolution_tessellation;
	int _v_tess = (num_vertices - 3) * curve_tessellation;
	TriangleNet* trinet = new TriangleNet(revolution_tessellation, _v_tess);

	for (int i = 0; i < num_vertices - 3; i++)
	{
		for (int j = 0; j <= curve_tessellation; j++)
		{
			float t = j * 1.0f / curve_tessellation;
			float x = vertices[i].x() * pow(1.0f - t, 3) / 6 + vertices[1 + i].x() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].x() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].x() * pow(t, 3) / 6;
			float y = vertices[i].y() * pow(1.0f - t, 3) / 6 + vertices[1 + i].y() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].y() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].y() * pow(t, 3) / 6;
			float z = vertices[i].z() * pow(1.0f - t, 3) / 6 + vertices[1 + i].z() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].z() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].z() * pow(t, 3) / 6;

			for (int k = 0; k <= revolution_tessellation; k++)
			{
				float x2 = z * sin(k * degree) + x * cos(k * degree);
				float y2 = y;
				float z2 = z * cos(k * degree) - x * sin(k * degree);
				trinet->SetVertex(k, j + i * curve_tessellation, Vec3f(x2, y2, z2));
			}
		}
	}
	return trinet;
}

void BSplineCurve::drawCurve(ArgParser* args)
{
	curve_tessellation = args->curve_tessellation;
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < num_vertices - 3; i++)
	{
		for (int j = 0; j <= curve_tessellation; j++)
		{
			float t = 1.0f * j / curve_tessellation;
			float x = vertices[i].x() * pow(1.0f - t, 3) / 6 + vertices[1 + i].x() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].x() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].x() * pow(t, 3) / 6;
			float y = vertices[i].y() * pow(1.0f - t, 3) / 6 + vertices[1 + i].y() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].y() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].y() * pow(t, 3) / 6;
			float z = vertices[i].z() * pow(1.0f - t, 3) / 6 + vertices[1 + i].z() * (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6 + vertices[2 + i].z() * (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6 + vertices[3 + i].z() * pow(t, 3) / 6;

			//是否可以优化为递归计算?
			glVertex3f(x, y, z);

		}
	}
	glEnd();
}

