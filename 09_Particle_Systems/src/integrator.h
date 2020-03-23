#pragma once

#include "forceField.h"
#include "particle.h"

class Integrator {
public:
    Integrator() {}

    virtual ~Integrator() {}

    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;

    virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator {
public:
    EulerIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(1, 0, 0); }
};

class MidpointIntegrator : public Integrator {
public:
    MidpointIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(0, 1, 0); }

};

class RungeKuttaIntegrator : public Integrator {
public:
    RungeKuttaIntegrator() {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override { return Vec3f(0, 0, 1); }

};