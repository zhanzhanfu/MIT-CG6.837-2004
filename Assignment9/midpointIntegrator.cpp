#include "midpointIntegrator.h"

MidpointIntegrator::MidpointIntegrator()
{
}

MidpointIntegrator::~MidpointIntegrator()
{
}

void MidpointIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	float mass = particle->getMass();
	Vec3f position = particle->getPosition();
	color = particle->getColor();
	Vec3f velocity = particle->getVelocity();
	
	
	Vec3f pmid = position + velocity * (dt / 2);
	Vec3f vmid = velocity + forcefield->getAcceleration(position, mass, t) * (dt / 2);
	
	position = position + vmid * dt;
	velocity = velocity + forcefield->getAcceleration(pmid, mass, t + dt / 2) * dt;
	particle->setPosition(position);
	particle->setVelocity(velocity);
	particle->increaseAge(dt);
}
