#pragma once
#include "camera.h"

class OrthographicCamera :public Camera {
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float size;
public:
	OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s);
	virtual ~OrthographicCamera();
	virtual Ray generateRay(Vec2f point) override;
	virtual float getTMin() override;
};