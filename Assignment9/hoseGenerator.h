#pragma once
#include "generator.h"

class HoseGenerator :public Generator {
public:
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness);
	virtual ~HoseGenerator();

	// Í¨¹ý Generator ¼Ì³Ð
	virtual int numNewParticles(float current_time, float dt) const override;
	virtual Particle* Generate(float current_time, int i) override;
	virtual void Paint() const override;
private:
	Vec3f position;
	float position_randomness;
	Vec3f velocity;
	float velocity_randomness;

	
};