#pragma once
#include "forceField.h"

class WindForceField :public ForceField {
public:
	WindForceField();
	virtual ~WindForceField();

	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;
};