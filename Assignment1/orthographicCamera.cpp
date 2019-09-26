#include "orthographicCamera.h"

OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) 
	:center(c)
	,direction(d)
	,up(u)
	,size(s)
{
	
	Vec3f::Cross3(horizontal, direction, up);
	Vec3f::Cross3(up, horizontal, direction);
	direction.Normalize();
	up.Normalize();
	horizontal.Normalize();
}

OrthographicCamera::~OrthographicCamera() {

}


//(0,0)->(1,1)
Ray OrthographicCamera::generateRay(Vec2f point) {
	Vec3f c = center + (point.x() - 0.5) * size * horizontal + (point.y() - 0.5) * size * up;
	return Ray(c, direction);
}


float OrthographicCamera::getTMin() {
	return -INT_MAX;
}