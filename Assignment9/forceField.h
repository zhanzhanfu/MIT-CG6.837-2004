#pragma once

#include "vectors.h"
class ForceField {
public:
	ForceField(){}
	virtual ~ForceField(){}
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const = 0;

	
};