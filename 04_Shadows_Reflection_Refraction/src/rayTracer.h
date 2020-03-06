#pragma once

#include "scene_parser.h"
#include "object3d.h"
#include "light.h"
#include "rayTree.h"
extern bool shadows;
extern bool shade_back;

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float min_weight) : scene(s), max_bounces(max_bounces), min_weight(min_weight) {}

    ~RayTracer() {}

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    SceneParser *scene;
    int max_bounces;
    float min_weight;
};

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit) {

    Group *group = scene->getGroup();
    //shader
    if (group->intersect(ray, hit, tmin)) {
        if(bounces == 0){
            RayTree::SetMainSegment(ray, tmin, hit.getT());    //main---------------------------------
        }
        Vec3f color(0.0, 0.0, 0.0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();
        color += scene->getAmbientLight() * material->getDiffuseColor();

        for (int k = 0; k < scene->getNumLights(); ++k) {
            Light *light = scene->getLight(k);
            Vec3f l, lightColor;
            float dis; //not be used
            light->getIllumination(pos, l, lightColor, dis);
            //shadows
            Ray ray_shadows(pos, l);
            Hit hit_shadows(dis);
            if (shadows) {
                if (!group->intersectShadowRay(ray_shadows, hit_shadows, tmin))
                    color += material->Shade(ray, hit, l, lightColor);
            } else
                color += material->Shade(ray, hit, l, lightColor);
            RayTree::AddShadowSegment(ray_shadows, tmin, hit_shadows.getT());  //shadows---------------------------------

        }
        //reflective
        Ray scattered;
        Vec3f attenuation;
        if (bounces < max_bounces && weight > min_weight && material->reflect(ray, hit, attenuation, scattered)) {
            Hit hit_ref(INFINITY);
            color += attenuation * traceRay(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddReflectedSegment(scattered, tmin, hit_ref.getT());//reflect---------------------------------
        }
        //refraction transparent
        if (bounces < max_bounces && weight > min_weight && material->refract(ray, hit, attenuation, scattered)) {
            Hit hit_ref(INFINITY);
            color += attenuation * traceRay(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddTransmittedSegment(scattered, tmin, hit_ref.getT());//trans---------------------------------
        }
        return color;
    } else {
        return scene->getBackgroundColor();
    }
}


