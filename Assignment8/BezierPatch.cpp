#include "bezierPatch.h"

BezierPatch::BezierPatch()
{
	num_vertices = 16;
	vertices = new Vec3f[num_vertices];
}

BezierPatch::~BezierPatch()
{
}

void BezierPatch::Paint(ArgParser* args)
{
	drawLine();
	drawPoints();
	drawCurve(args);
}

TriangleMesh* BezierPatch::OutputTriangles(ArgParser* args)
{
	//不懂
	int patch_tessellation = args->patch_tessellation;
	TriangleNet* trinet = new TriangleNet(patch_tessellation, patch_tessellation);
	float m[16] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };
	Matrix m2(m);
	Matrix m3 = m2;
	m3.Transpose();
	for (int u = 0; u <= patch_tessellation; u++)
	{
		float u_tess = u * 1.0f / patch_tessellation;
		Vec3f ver;
		for (int v = 0; v <= patch_tessellation; v++)
		{
			float v_tess = v * 1.0f / patch_tessellation;
			float m_u[4] = { pow(u_tess, 3), pow(u_tess, 2), u_tess, 1 };
			Matrix mu(m_u);
			Matrix temp1 = mu * m2;
			Vec3f vs[4];
			for (int i = 0; i < 4; i++)
			{
				vs[i] = temp1.Get(0, 0) * vertices[0 + i] + temp1.Get(1, 0) * vertices[4 + i] + temp1.Get(2, 0) * vertices[8 + i] + temp1.Get(3, 0) * vertices[12 + i];
			}
			Vec3f vs2[4];
			for (int i = 0; i < 4; i++)
			{
				vs2[i] = vs[0] * m3.Get(0, i) + vs[1] * m3.Get(1, i) + vs[2] * m3.Get(2, i) + vs[3] * m3.Get(3, i);
			}
			ver = vs2[0] * pow(v_tess, 3) + vs2[1] * pow(v_tess, 2) + vs2[2] * v_tess + vs2[3];
			trinet->SetVertex(v, u, ver);
		}
	}
	return trinet;
}

void BezierPatch::drawCurve(ArgParser* args)
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


