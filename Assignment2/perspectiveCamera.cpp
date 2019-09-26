#include "perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float ang) {
	center = c;
	direction = d;
	up = u;
	angle = ang;
	Vec3f::Cross3(horizontal, direction, up);
	Vec3f::Cross3(up, horizontal, direction);
	direction.Normalize();
	up.Normalize();
	horizontal.Normalize();

}
PerspectiveCamera::~PerspectiveCamera() {

}
Ray PerspectiveCamera::generateRay(Vec2f point) {
	float distance = 0.5f / tanf(angle / 2);
	Vec3f dir = distance * direction + (point.x() - 0.5) * horizontal + (point.y() - 0.5) * up;
	
	dir.Normalize();

	return Ray(center, dir);
}
float PerspectiveCamera::getTMin() {
	return 0;
}