#include "ringGenerator.h"

RingGenerator::RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness)
	:position_randomness(position_randomness)
	,velocity(velocity)
	,velocity_randomness(velocity_randomness)
{
}

RingGenerator::~RingGenerator()
{
}

int RingGenerator::numNewParticles(float current_time, float dt) const
{
	float n = dt * desired_num_particles / lifespan + current_time;
	return n;
}

Particle* RingGenerator::Generate(float current_time, int i)
{
	float radius = 3.5f + (1.0f * random->next() - 0.5f);
	float theta = random->next() * 2.0f * 3.14159f;
	Vec3f position_random = Vec3f(radius * sinf(theta), 0.0f, radius * cosf(theta));
	Vec3f velocity_random = velocity + Vec3f(0.707f * (2 * random->next() - 1.0f) * velocity_randomness, 0.707f * (2 * random->next() - 1.0f) * velocity_randomness, 0.0f);
	float mass_random = mass + (2 * random->next() - 1.0f) * mass_randomness;
	float lifespan_random = lifespan + (2 * random->next() - 1.0f) * lifespan_randomness;
	Particle* particles = new Particle(position_random, velocity_random, color, dead_color, mass_random, lifespan_random);
	return particles;
	
}

void RingGenerator::Paint() const
{
	glBegin(GL_QUADS);
	glVertex3f(5.0f, 0.0f, 5.0f);
	glVertex3f(-5.0f, 0.0f, 5.0f);
	glVertex3f(-5.0f, 0.0f, -5.0f);
	glVertex3f(5.0f, 0.0f, -5.0f);
	glEnd();
}
