#pragma once
#include "forceField.h"

class VerticalForceField :public ForceField {
public:
	VerticalForceField(float magnitude);
	virtual ~VerticalForceField();

	// Í¨¹ý ForceField ¼Ì³Ð
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;
private:
	float magnitude;
};