#include "vectors.h"


class Sampler {
public:
    Sampler(int num_samples) : num_samples(num_samples) {}

    virtual Vec2f getSamplePosition(int n) = 0;

protected:
    int num_samples;
};

class RandomSampler : public Sampler {
public:
    RandomSampler(int num_samples) : Sampler(num_samples) {}

    Vec2f getSamplePosition(int n) override {
        return Vec2f(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX);
    }
};


class UniformSampler : public Sampler {
public:
    UniformSampler(int num_samples) : Sampler(num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / (size + 1);
    }

    Vec2f getSamplePosition(int n) override {
        if (num_samples == 1)
            return Vec2f();
        return Vec2f((n % size + 1) * d, (n / size + 1) * d);
    }

private:
    int size;
    float d;
};

class JitteredSampler : public Sampler {
public:
    JitteredSampler(int num_samples) : Sampler(num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / (size + 1);
    }

    Vec2f getSamplePosition(int n) override {
        Vec2f p = Vec2f((n % size + 1) * d, (n / size + 1) * d);
        return Vec2f(p.x() + (2.0f * rand() / RAND_MAX - 1) * d, p.y() + (2.0f * rand() / RAND_MAX - 1) * d);
    }

private:
    int size;
    float d;
};