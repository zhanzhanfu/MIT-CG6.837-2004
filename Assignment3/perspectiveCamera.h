#pragma once
#include "camera.h"

class PerspectiveCamera :public Camera {
private:
	float angle;
public:
	PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float ang);
	virtual ~PerspectiveCamera();
	virtual Ray generateRay(Vec2f point) override;
	virtual float getTMin() override;

	virtual void glInit(int w, int h) override;
	virtual void glPlaceCamera(void) override;
	virtual void dollyCamera(float dist) override;
	virtual void truckCamera(float dx, float dy) override;
	virtual void rotateCamera(float rx, float ry) override;
};