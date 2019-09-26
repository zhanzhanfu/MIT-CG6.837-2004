#include "sphere.h"
const float PI = M_PI;
extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
Sphere::Sphere(Vec3f c, float r, Material *m) {
	center = c;
	radius = r;
	material = m;
}
Sphere::~Sphere(){
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;

	Vec3f origin = r.getOrigin();
	Vec3f direction = r.getDirection();

	float a = direction.Dot3(direction);
	float b = 2.0f * direction.Dot3(origin - center);
	float c = (origin - center).Length() * (origin - center).Length() - radius * radius;
	float discriminant = b * b - 4 * a*c;
	float t = INT_MAX * 1.0f;
	if (discriminant >= 0) {
		float d = sqrt(discriminant);
		float t1 = (-b - d) / (a * 2.0f);
		float t2 = (-b + d) / (a * 2.0f);

		if (t1 >= tmin) {
			t = t1;
			flag = true;
		}
		else if (t2 >= tmin) {
			t = t2;
			flag = true;
		}
		if (t < h.getT()) {
			Vec3f normal = origin + direction * t - center;
			normal.Normalize();
			h.set(t, material, normal, r);

		}
		
	}
	return  flag;
}

void Sphere::paint() { 
	//²»¶®
	glSetMaterial();
	float theta = 2 * PI / theta_steps;
	float phi = PI / phi_steps;
	Vec3f *points = new Vec3f[theta_steps*(phi_steps + 1)];

	// initialize all the position for points
	Vec3f top = center + Vec3f(0, 1, 0)*radius;
	Vec3f below = center + Vec3f(0, -1, 0)*radius;
	for (int iPhi = 0; iPhi < phi_steps + 1; iPhi++)
	{
		for (int iTheta = 0; iTheta < theta_steps; iTheta++)
		{
			int index = theta_steps * iPhi + iTheta;
			if (iPhi == 0)
			{
				points[index] = below;
				continue;
			}
			if (iPhi == phi_steps)
			{
				points[index] = top;
				continue;
			}
			float tphi = iPhi * phi - PI / 2;
			float ttheta = theta * iTheta;
			points[index] = center + radius * sin(tphi)*Vec3f(0, 1, 0)
				+ radius * cos(tphi)*cos(ttheta)*Vec3f(1, 0, 0) + radius * cos(tphi)*sin(ttheta)*Vec3f(0, 0, 1);
		}
	}

	glBegin(GL_QUADS);
	for (int iPhi = 0; iPhi < phi_steps; iPhi++)
	{
		for (int iTheta = 0; iTheta < theta_steps; iTheta++)
		{
			//glNormal3f(normal.x(), normal.y(), normal.z());
			int index[4] = { theta_steps * iPhi + iTheta ,theta_steps * iPhi + (iTheta + 1) % theta_steps,
				theta_steps * (iPhi + 1) + (iTheta + 1) % theta_steps ,theta_steps * (iPhi + 1) + iTheta };
			Vec3f normal;
			if (!gouraud)
			{
				Vec3f::Cross3(normal, points[index[0]] - points[index[1]], points[index[2]] - points[index[1]]);
				if (normal.Length() == 0)
				{
					Vec3f::Cross3(normal, points[index[1]] - points[index[2]], points[index[3]] - points[index[2]]);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (gouraud)
				{
					normal = points[index[i]] - center;
				}
				normal.Normalize();
				glNormal3f(normal.x(), normal.y(), normal.z());

				glVertex3f(points[index[i]].x(), points[index[i]].y(), points[index[i]].z());

			}
		}
	}

	glEnd();
}