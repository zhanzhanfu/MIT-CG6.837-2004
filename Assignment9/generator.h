#pragma once
#include "particle.h"
#include "random.h"
#include <GL/freeglut.h>

class Generator {
public:

	Generator();
	virtual ~Generator();
	// initialization
	virtual void SetColors(Vec3f color, Vec3f dead_color, float color_randomness);
	virtual void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles);
	virtual void SetMass(float mass, float mass_randomness);

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const = 0;
	virtual Particle* Generate(float current_time, int i) = 0;

	// for the gui
	virtual void Paint() const = 0;
	virtual void Restart();

protected:
	Vec3f color;
	Vec3f dead_color;
	float color_randomness;
	float lifespan;
	float lifespan_randomness;
	int desired_num_particles;
	float mass;
	float mass_randomness;
	Random* random;
};