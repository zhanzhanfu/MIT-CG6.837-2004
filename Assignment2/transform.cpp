#include "transform.h"

Transform::Transform(Matrix &m, Object3D *o) {
	obj = o;
	matrix = m;
}
Transform::~Transform() {

}
bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;
	Vec3f origin = r.getOrigin();
	Vec3f direction = r.getDirection();
	Matrix invM = matrix;

	if (invM.Inverse()) {
		invM.Transform(origin);
		invM.TransformDirection(direction);

		//为什么不能单位化
		//direction.Normalize();
		
		Ray r2(origin, direction);
		flag = obj->intersect(r2, h, tmin);
		if (flag) {
			Vec3f n = h.getNormal();
			Matrix invMT = invM;
			invMT.Transpose();
			invMT.TransformDirection(n);
			n.Normalize();
			h.set(h.getT(), h.getMaterial(), n, r);
		}
	}
	return flag;
}
