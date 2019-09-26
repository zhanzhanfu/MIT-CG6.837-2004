#include "eulerIntegrator.h"

EulerIntegrator::EulerIntegrator()
{
}

EulerIntegrator::~EulerIntegrator()
{
}

void EulerIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	float mass = particle->getMass();
	color = particle->getColor();
	Vec3f position = particle->getPosition();
	Vec3f velocity = particle->getVelocity();

	Vec3f a = forcefield->getAcceleration(position, mass, t);
	position = position + velocity * dt;
	particle->setPosition(position);
	velocity = velocity + a * dt;
	particle->setVelocity(velocity);
	particle->increaseAge(dt);
}
