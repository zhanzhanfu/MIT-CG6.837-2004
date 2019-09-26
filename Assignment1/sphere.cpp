#include "sphere.h"

Sphere::Sphere(Vec3f c, float r, Material *m) {
	center = c;
	radius = r;
	material = m;
}
Sphere::~Sphere(){
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;

	Vec3f ro = r.getOrigin() - center;
	Vec3f rd = r.getDirection();

	float a = rd.Dot3(rd);
	float b = 2 * ro.Dot3(rd);
	float c = ro.Dot3(ro) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	float t = INT_MAX * 1.0f;
	if (discriminant >= 0) {
		float d = sqrt(discriminant);
		float t1 = (-b - d) *0.5f;
		float t2 = (-b + d) *0.5f;

		if (t1 >= tmin) {
			t = t1;
			flag = true;
		}
		else if (t2 >= tmin) {
			t = t2;
			flag = true;
		}
		if (t < h.getT()) {
			h.set(t, material, r);
		}
		
	}
	return  flag;
}