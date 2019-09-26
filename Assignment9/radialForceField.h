#pragma once
#include "forceField.h"

class RadialForceField :public ForceField {

public:
	RadialForceField(float magnitude);
	virtual ~RadialForceField();



	// Í¨¹ý ForceField ¼Ì³Ð
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

private:
	float magnitude;
};