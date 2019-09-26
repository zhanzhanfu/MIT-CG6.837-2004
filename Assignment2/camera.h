#pragma once
#include "ray.h"

class Camera {

public:
	Camera(){}
	virtual ~Camera(){}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() = 0;
};