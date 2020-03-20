#pragma once

#include "film.h"

class Filter {
public:
    Filter() {}

    Vec3f getColor(int i, int j, Film *film);

    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {
public:
    BoxFilter(float radius) : radius(radius) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

private:
    float radius;
};


class TentFilter : public Filter {
public:
    TentFilter(float radius) : radius(radius) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

private:
    float radius;
};

class GaussianFilter : public Filter {
public:
    GaussianFilter(float radius) : radius(radius) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override;

private:
    float radius;
};