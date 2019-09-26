#pragma once
#include "object3d.h"

class Plane :public Object3D {
private:
	Vec3f normal;
	float d;
public:
	Plane(Vec3f &normal, float d, Material *m);
	virtual ~Plane();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
	virtual void paint() override;
};