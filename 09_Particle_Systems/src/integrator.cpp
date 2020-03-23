#include "integrator.h"

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f p = particle->getPosition();
    float m = particle->getMass();
    Vec3f v = particle->getVelocity();
    Vec3f a = forcefield->getAcceleration(p, m, t);
    Vec3f p_new = p + v * dt;
    Vec3f v_new = v + a * dt;
    particle->setPosition(p_new);
    particle->setVelocity(v_new);
    particle->increaseAge(dt);
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f p = particle->getPosition();
    float m = particle->getMass();
    Vec3f v = particle->getVelocity();
    Vec3f a = forcefield->getAcceleration(p, m, t);
    Vec3f pm = p + v * (dt / 2);
    Vec3f vm = v + a * (dt / 2);

    Vec3f am = forcefield->getAcceleration(pm, m, t + dt / 2);
    Vec3f p_new = p + vm * dt;
    Vec3f v_new = v + am * dt;
    particle->setPosition(p_new);
    particle->setVelocity(v_new);
    particle->increaseAge(dt);
}

void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f p = particle->getPosition();
    float m = particle->getMass();
    Vec3f v = particle->getVelocity();
    Vec3f a = forcefield->getAcceleration(p, m, t);

    Vec3f pa = p + v * dt;
    Vec3f va = v + a * dt;
    Vec3f pb = p + va * dt;
    particle->setPosition((pa + pb) * 0.5);
    particle->setVelocity((v + va) * 0.5);
    particle->increaseAge(dt);
}
