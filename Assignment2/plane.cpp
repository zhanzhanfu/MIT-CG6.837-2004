#include "plane.h"

Plane::Plane(Vec3f &normal, float d, Material *m) {
	this->normal = normal;
	this->d = d;
	material = m;

}
Plane::~Plane() {

}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;
	Vec3f origin = r.getOrigin();
	Vec3f direction = r.getDirection();
	if (normal.Dot3(direction) == 0) {
		flag = false;
		return flag;
	}
	float t = (d - normal.Dot3(origin)) / normal.Dot3(direction);
	if (t > tmin && t < h.getT()) {
		h.set(t, material, normal, r);
		flag = true;
	}
	return  flag;
}