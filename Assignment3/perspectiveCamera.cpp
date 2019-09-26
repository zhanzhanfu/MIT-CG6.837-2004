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
	Vec3f dir = distance * direction + (point.x() - 0.5)*horizontal + (point.y() - 0.5)*up;
	
	dir.Normalize();

	return Ray(center, dir);
}
float PerspectiveCamera::getTMin() {
	return 0;
}

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle*180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
	gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
	center += direction * dist;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal * dx + screenUp * dy;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
	direction.Normalize();

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}