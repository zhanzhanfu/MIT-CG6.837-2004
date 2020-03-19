#pragma once

#include "scene_parser.h"
#include "object3d.h"
#include "grid.h"

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float min_weight);

    ~RayTracer();

    Vec3f TraceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    Grid *getGrid() { return grid; }

    Vec3f RayCast(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    Vec3f RayCastFast(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

private:
    SceneParser *scene;
    int max_bounces;
    float min_weight;
    Grid *grid;
};



