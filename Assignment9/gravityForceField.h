#pragma once
#include "forceField.h"

class GravityForceField :public ForceField {
public:
	GravityForceField(Vec3f gravity);
	virtual ~GravityForceField();

	// Í¨¹ý ForceField ¼Ì³Ð
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

private:
	Vec3f gravity;
};