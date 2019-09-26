#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
class Object3D {
protected:
	Material *material;

public:
	Object3D(){}
	virtual ~Object3D() {}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
	//Shadow Optimization:Stops at first intersection
	//virtual bool intersectShadowRay(const Ray &r, Hit &h, float tmin) = 0;
};