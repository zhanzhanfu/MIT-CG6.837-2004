#pragma once
#include "integrator.h"

class MidpointIntegrator :public Integrator {
public:
	MidpointIntegrator();
	virtual ~MidpointIntegrator();

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt);


};