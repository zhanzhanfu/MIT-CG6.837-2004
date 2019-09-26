#pragma once
#include "ray.h"
#include "hit.h"
class Object3D {
protected:
	Material *material;

public:
	Object3D(){}
	virtual ~Object3D() {}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

};