#pragma once
#include "vectors.h"
#include "particle.h"
#include "forceField.h"

class Integrator {
public:
	Integrator(){}
	virtual ~Integrator(){}
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	Vec3f getColor() { return color; }
protected:
	Vec3f color;
};