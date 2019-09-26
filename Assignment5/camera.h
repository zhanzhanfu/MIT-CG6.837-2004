#pragma once
#include "ray.h"
#include "matrix.h"
#include <GL/freeglut.h>

class Camera {
protected:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
public:
	Camera(){}
	virtual ~Camera(){}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() = 0;

	//initializing, placing, dollying, trucking, and rotating an OpenGL camera
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
};