#include "hoseGenerator.h"

HoseGenerator::HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness)
	:position(position)
	,position_randomness(position_randomness)
	,velocity(velocity)
	,velocity_randomness(velocity_randomness)
{

}

HoseGenerator::~HoseGenerator()
{
}

int HoseGenerator::numNewParticles(float current_time, float dt) const
{
	float n = dt * desired_num_particles / lifespan;
	return n;
}

Particle* HoseGenerator::Generate(float current_time, int i)
{
	//Vec3f position_random = position + random->randomVector() * position_randomness * 0.707f;
	//Vec3f velocity_random = velocity + random->randomVector() * velocity_randomness * 0.707f;
	Vec3f position_random = position + random->randomVector() * position_randomness;
	Vec3f velocity_random = velocity + random->randomVector() * velocity_randomness;
	float mass_random = mass + (2 * random->next() - 1.0f) * mass_randomness;
	float lifespan_random = lifespan + (2 * random->next() - 1.0f) * lifespan_randomness;
	Particle* p = new Particle(position_random, velocity_random, color, dead_color, mass_random, lifespan_random);
	return p;
}

void HoseGenerator::Paint() const
{
}
