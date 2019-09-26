#include "rungeKuttaIntegrator.h"

RungeKuttaIntegrator::RungeKuttaIntegrator()
{
}

RungeKuttaIntegrator::~RungeKuttaIntegrator()
{
}

void RungeKuttaIntegrator::Update(Particle* particle, ForceField* forcefield, float t, float dt)
{
	Vec3f position_current = particle->getPosition();
	Vec3f velocity_current = particle->getVelocity();
	color = particle->getColor();
	Vec3f position_mid = position_current + velocity_current * (dt * 0.5f);
	Vec3f velocity_mid = velocity_current + forcefield->getAcceleration(position_current, particle->getMass(), t) * (dt * 0.5f);
	Vec3f position_next = position_current + velocity_mid * dt;
	Vec3f velocity_next = velocity_current + forcefield->getAcceleration(position_mid, particle->getMass(), t + 0.5f * dt) * dt;
	particle->increaseAge(dt);
	particle->setPosition(position_next);
	particle->setVelocity(velocity_next);


	/*float mass = particle->getMass();
	color = particle->getColor();
	Vec3f position = particle->getPosition();
	Vec3f velocity = particle->getVelocity();


	Vec3f pmid = position + velocity * (dt / 2);
	Vec3f vmid = velocity + forcefield->getAcceleration(position, mass, t) * (dt / 2);

	position = position + vmid * dt;
	velocity = velocity + forcefield->getAcceleration(pmid, mass, t + dt / 2) * dt;
	particle->setPosition(position);
	particle->setVelocity(velocity);
	particle->increaseAge(dt);*/
	
}
