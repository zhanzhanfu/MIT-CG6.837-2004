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