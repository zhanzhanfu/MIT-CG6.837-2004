#pragma once

#include "object3d.h"
class Sphere :public Object3D {

public:
	Sphere(Vec3f c, float r, Material* m);
	virtual ~Sphere();
	virtual bool intersect(const Ray& r, Hit& h, float tmin);

private:
	Vec3f center;
	float radius;
};