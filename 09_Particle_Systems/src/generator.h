#pragma once

#include "particle.h"
#include "random.h"

class Generator {
public:
    Generator() { random = new Random(); }

    virtual ~Generator() { delete random; }

    // initialization
    void SetColors(const Vec3f &color, const Vec3f &dead_color, float color_randomness);

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles);

    void SetMass(float mass, float mass_randomness);

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;

    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const {}

    void Restart() {delete random; random = new Random;}

protected:
    Vec3f color;
    Vec3f dead_color;
    float color_randomness;
    float lifespan;
    float lifespan_randomness;
    int desired_num_particles;
    float mass;
    float mass_randomness;
    Random *random;
};

class HoseGenerator : public Generator {
public:
    HoseGenerator(const Vec3f &position, float position_randomness, const Vec3f &velocity, float velocity_randomness) :
            position(position), position_randomness(position_randomness), velocity(velocity), velocity_randomness(velocity_randomness) {}

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

private:
    Vec3f position;
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
};

class RingGenerator : public Generator {
public:
    RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) :
            position_randomness(position_randomness), velocity(velocity), velocity_randomness(velocity_randomness) {}

    int numNewParticles(float current_time, float dt) const override;

    Particle *Generate(float current_time, int i) override;

    void Paint() const override;

private:
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
};

