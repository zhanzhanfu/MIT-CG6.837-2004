#pragma once

#include "object3d.h"

class Group :public Object3D {
private:
	int num_obj;
	Object3D** object3Ds;
public:
	Group(int num);
	virtual ~Group();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
	void addObject(int index, Object3D *obj);
	virtual void paint() override;
};