#pragma once
#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "rayTree.h"

class RayTracer {
private:
	SceneParser *sp;
	int max_bounces;
	float cutoff_weight;
	bool shadows;
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows);
	~RayTracer();
	
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit &hit) const;
	Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const;
	bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const;

};