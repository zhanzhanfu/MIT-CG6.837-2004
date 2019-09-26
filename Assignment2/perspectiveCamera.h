#pragma once
#include "camera.h"

class PerspectiveCamera :public Camera {
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float angle;
public:
	PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float ang);
	virtual ~PerspectiveCamera();
	virtual Ray generateRay(Vec2f point) override;
	virtual float getTMin() override;
};