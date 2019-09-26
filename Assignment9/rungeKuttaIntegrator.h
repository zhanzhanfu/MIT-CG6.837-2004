#pragma once
#include "integrator.h"

class RungeKuttaIntegrator :public Integrator {
public:
	RungeKuttaIntegrator();
	virtual ~RungeKuttaIntegrator();
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
};