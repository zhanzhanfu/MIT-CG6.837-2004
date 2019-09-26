#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "boundingbox.h"


class Grid;
class Object3D {
protected:
	
	BoundingBox *boundingBox;

public:
	Object3D(){}
	virtual ~Object3D() { delete material; }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
	virtual BoundingBox* getBoundingBox() { return NULL; }
	virtual void insertIntoGrid(Grid *grid, Matrix *m) {}
	//Shadow Optimization:Stops at first intersection
	//virtual bool intersectShadowRay(const Ray &r, Hit &h, float tmin) = 0;

	Material *material;
};