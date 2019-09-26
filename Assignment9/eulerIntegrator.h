#pragma once
#include "integrator.h"

class EulerIntegrator :public Integrator {
public:
	EulerIntegrator();
	virtual ~EulerIntegrator();

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;

private:

};