#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
class Object3D {
protected:
	Material *material;

public:
	Object3D(){}
	virtual ~Object3D() { delete material; }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
	virtual void glSetMaterial() const { material->glSetMaterial(); };
};