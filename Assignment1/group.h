#pragma once

#include "object3d.h"

class Group :public Object3D {

public:
	Group(int num);
	virtual ~Group();
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	void addObject(int index, Object3D* obj);
	

private:
	int num_obj;
	Object3D** object3Ds;
};