#pragma once
#include "camera.h"

class OrthographicCamera :public Camera {
private:
	float size;
public:
	OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s);
	virtual ~OrthographicCamera();
	virtual Ray generateRay(Vec2f point) override;
	virtual float getTMin() override;

	virtual void glInit(int w, int h) override;
	virtual void glPlaceCamera(void) override;
	virtual void dollyCamera(float dist) override;
	virtual void truckCamera(float dx, float dy) override;
	virtual void rotateCamera(float rx, float ry) override;
};