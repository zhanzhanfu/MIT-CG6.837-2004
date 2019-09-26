#include "group.h"

Group::Group(int num) {
	num_obj = num;
	object3Ds = new Object3D*[num_obj];
	isUseGrid = false;
	this->boundingBox = new BoundingBox(Vec3f(INT_MAX, INT_MAX, INT_MAX), Vec3f(INT_MIN, INT_MIN, INT_MIN));
}
Group::~Group() {

}
bool Group::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;
	if (isUseGrid) {
		flag =grid->intersect(r, h, tmin);
	}
	else {
		for (int i = 0; i < num_obj; i++)
		{
			if (object3Ds[i]->intersect(r, h, tmin)) {
				flag = true;
			}
		}
	}
	
	return flag;
}
void Group::addObject(int index, Object3D *obj) {
	object3Ds[index] = obj;
	if (obj->getBoundingBox() != NULL) {
		boundingBox->Extend(obj->getBoundingBox());
	}
}

void Group::paint() {
	for (int i=0;i<num_obj;i++)
	{
		object3Ds[i]->paint();
	}
}

void Group::insertIntoGrid(Grid *grid, Matrix *m) {
	this->grid = grid;
	for (int i=0;i<num_obj;i++)
	{
		if (object3Ds[i]->getBoundingBox() != NULL) {
			object3Ds[i]->insertIntoGrid(grid, m);
		}
	}
}

BoundingBox* Group::getBoundingBox() {
	return boundingBox;
}

void Group::setIsGrid(bool isUseGrid) {
	this->isUseGrid = isUseGrid;
}