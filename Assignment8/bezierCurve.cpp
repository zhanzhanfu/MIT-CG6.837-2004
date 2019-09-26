#include "bezierCurve.h"

BezierCurve::BezierCurve(int num_v)
{
	num_vertices = num_v;
	vertices = new Vec3f[num_vertices];
	type_id = 1;
}

BezierCurve::~BezierCurve()
{
	delete[] vertices;
}

void BezierCurve::OutputBezier(FILE* file)
{
	fprintf(file, "bezier\n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		fprintf(file, "%f ", vertices[i].x());
		fprintf(file, "%f ", vertices[i].y());
		fprintf(file, "%f\n", vertices[i].z());
	}
}

void BezierCurve::OutputBSpline(FILE* file)
{
	
	Matrix gbz, gbs;
	for (int i = 0; i < num_vertices; i++)
	{
		//gbz.Set(0, i, vertices[i].x());
		//gbz.Set(1, i, vertices[i].y());
		//gbz.Set(2, i, vertices[i].z());
		gbz.Set(i, 0, vertices[i].x());   //为什么控制点是行
		gbz.Set(i, 1, vertices[i].y());
		gbz.Set(i, 2, vertices[i].z());
	}
	float a_bbz[16] = { -1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0 };
	float a_bbs[16] = { -1,3,-3,1,3,-6,0,4,-3,3,3,1,1,0,0,0 };

	Matrix bbz(a_bbz);
	Matrix bbs(a_bbs);
	bbs = bbs * (1.0f/6);
	bbs.Inverse();
	gbs = gbz * bbz * bbs;
	fprintf(file, "bspline\n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		fprintf(file, "%f ", gbs.Get(i, 0));
		fprintf(file, "%f ", gbs.Get(i, 1));
		fprintf(file, "%f\n", gbs.Get(i, 2));
	}
}

void BezierCurve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set(x, y, vertices[selectedPoint].z());
}

void BezierCurve::addControlPoint(int selectedPoint, float x, float y)
{
	num_vertices += 3;
	Vec3f *temp = new Vec3f[num_vertices];
	for (int i = 0; i < selectedPoint; i++)
	{
		temp[i] = vertices[i];
	}
	temp[selectedPoint] = Vec3f(x, y, 0);
	temp[selectedPoint + 1] = Vec3f(x, y, 0);
	temp[selectedPoint + 2] = Vec3f(x, y, 0);
	for (int i = selectedPoint+3; i < num_vertices; i++)
	{
		temp[i] = vertices[i-3];
	}
	delete[] vertices;
	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}
	delete[] temp;
	
}

void BezierCurve::deleteControlPoint(int selectedPoint)
{
	if (num_vertices <= 4)
		return;

	int pre = selectedPoint - 1;
	int next = selectedPoint + 1;
	if (pre < 0) {
		pre++;
		next++;
	}
	if (next > num_vertices - 1) {
		pre--;
		next--;
	}
	
	for (int i = next+1; i < num_vertices; i++)
	{
		vertices[i - 3] = vertices[i];
	}
	num_vertices -= 3;
	
}

TriangleMesh* BezierCurve::OutputTriangles(ArgParser* args)
{
	curve_tessellation = args->curve_tessellation;
	int revolution_tessellation = args->revolution_tessellation;
	float degree = 2 * M_PI / revolution_tessellation;
	int _v_tess = num_vertices / 3 * curve_tessellation;
	TriangleNet* trinet = new TriangleNet(revolution_tessellation, _v_tess);
	for (int i = 0; i < num_vertices - 3; i += 3)
	{
		for (float j = 0; j <= curve_tessellation; j++)
		{
			float t = j * 1.0f / curve_tessellation;
			Vec3f p01 = vertices[i] * (1 - t) + vertices[i + 1] * t;
			Vec3f p11 = vertices[i + 1] * (1 - t) + vertices[i + 2] * t;
			Vec3f p21 = vertices[i + 2] * (1 - t) + vertices[i + 3] * t;
			Vec3f p02 = p01 * (1 - t) + p11 * t;
			Vec3f p12 = p11 * (1 - t) + p21 * t;
			Vec3f p03 = p02 * (1 - t) + p12 * t;

			for (int k = 0; k <= revolution_tessellation; k++)
			{
				float x = p03.z() * sin(k * degree) + p03.x() * cos(k * degree);
				float y = p03.y();
				float z = p03.z() * cos(k * degree) - p03.x() * sin(k * degree);
				trinet->SetVertex(k, j + (i / 3) * curve_tessellation, Vec3f(x, y, z));
			}
		}
	}
	return trinet;
}

void BezierCurve::drawCurve(ArgParser* args)
{
	curve_tessellation = args->curve_tessellation;
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < num_vertices - 3; i += 3)
	{
		for (int j = 0; j <= curve_tessellation; j++)
		{
			//GLfloat x = vertices[i].x() * pow(1.0f - t, 3) + 3 * vertices[1 + i].x() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].x() * t * t * (1.0f - t) + vertices[3 + i].x() * pow(t, 3);
			//GLfloat y = vertices[i].y() * pow(1.0f - t, 3) + 3 * vertices[1 + i].y() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].y() * t * t * (1.0f - t) + vertices[3 + i].y() * pow(t, 3);
			//GLfloat z = vertices[i].z() * pow(1.0f - t, 3) + 3 * vertices[1 + i].z() * t * pow(1.0f - t, 2) + 3 * vertices[2 + i].z() * t * t * (1.0f - t) + vertices[3 + i].z() * pow(t, 3);
			//算术优化为递归计算

			float t = 1.0 * j / curve_tessellation;
			Vec3f p01 = vertices[i] * (1 - t) + vertices[i + 1] * t;
			Vec3f p11 = vertices[i + 1] * (1 - t) + vertices[i + 2] * t;
			Vec3f p21 = vertices[i + 2] * (1 - t) + vertices[i + 3] * t;
			Vec3f p02 = p01 * (1 - t) + p11 * t;
			Vec3f p12 = p11 * (1 - t) + p21 * t;
			Vec3f p03 = p02 * (1 - t) + p12 * t;

			glVertex3f(p03.x(), p03.y(), p03.z());

		}
	}
	glEnd();
}

void BezierCurve::Paint(ArgParser* args)
{
	drawLine();
	drawPoints();
	drawCurve(args);
}


