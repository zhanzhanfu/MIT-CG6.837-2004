#pragma once
#include "object3d.h"
#include "matrix.h"

class Transform :public Object3D {
private:
	Matrix matrix;
	Object3D *obj;

public:
	Transform(Matrix &m, Object3D *o);
	virtual ~Transform();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
};