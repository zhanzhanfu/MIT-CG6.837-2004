#include "vectors.h"


class Sampler {
public:
    Sampler();

    virtual Vec2f getSamplePosition(int n) = 0;

private:

};

class RandomSampler : public Sampler {
public:
    RandomSampler();
};

class UniformSampler : public Sampler {
public:
    UniformSampler();
};

class JitteredSampler : public Sampler {
public:
};