#pragma once
#include "forceField.h"

class ConstantForceField :public ForceField {
public:
	ConstantForceField(Vec3f force);
	virtual ~ConstantForceField();



	// Í¨¹ý ForceField ¼Ì³Ð
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

private:
	Vec3f force;
};