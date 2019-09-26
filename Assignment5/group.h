#pragma once

#include "object3d.h"
#include "grid.h"
class Group :public Object3D {
private:
	int num_obj;
	Object3D **object3Ds;
	Grid *grid;
	bool isUseGrid;
public:
	Group(int num);
	virtual ~Group();
	virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
	virtual void paint() override;
	virtual void insertIntoGrid(Grid *grid, Matrix *m) override;
	virtual BoundingBox* getBoundingBox() override;
	
	void setIsGrid(bool isUseGrid);
	void addObject(int index, Object3D* obj);
};