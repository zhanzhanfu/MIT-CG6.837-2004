#include "group.h"

Group::Group(int num) {
	num_obj = num;
	object3Ds = new Object3D*[num_obj];
}
Group::~Group() {
	for (int i = 0; i < num_obj; i++)
	{
		delete object3Ds[i];
	}
	delete[] object3Ds;
}
bool Group::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;
	for (int i=0;i<num_obj;i++)
	{
		if (object3Ds[i]->intersect(r, h, tmin)) {
			flag = true;
		}
	}
	return flag;
}
void Group::addObject(int index, Object3D *obj) {
	object3Ds[index] = obj;
}