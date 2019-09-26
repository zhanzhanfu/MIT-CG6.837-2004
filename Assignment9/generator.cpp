#include "generator.h"

Generator::Generator()
{
	random = new Random(100);
}

Generator::~Generator()
{
	delete random;
}

void Generator::SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
{
	this->color = color;
	this->dead_color = dead_color;
	this->color_randomness = color_randomness;
}

void Generator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
{
	this->lifespan = lifespan;
	this->lifespan_randomness = lifespan_randomness;
	this->desired_num_particles = desired_num_particles;
}

void Generator::SetMass(float mass, float mass_randomness)
{
	this->mass = mass;
	this->mass_randomness = mass_randomness;
}

void Generator::Restart()
{
	delete random;
	random = new Random(100);
}
