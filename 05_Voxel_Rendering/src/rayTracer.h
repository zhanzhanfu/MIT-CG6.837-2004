#pragma once

#include "scene_parser.h"
#include "object3d.h"
#include "light.h"
#include "rayTree.h"
#include "grid.h"

extern bool shadows;
extern bool shade_back;
extern bool visualize_grid;
extern int nx, ny, nz;

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float min_weight) : scene(s), max_bounces(max_bounces), min_weight(min_weight) {
        if (nx != 0) {
            grid = new Grid(s->getGroup()->getBoundingBox(), nx, ny, nz);
            s->getGroup()->insertIntoGrid(grid, nullptr);
            //print
            //int ans = 0;
            //for (int k = 0; k < nz; ++k) {
            //    for (int j = 0; j < ny; ++j) {
            //        for (int i = 0; i < nx; ++i) {
            //            int index = nx * ny * k + nx * j + i;
            //            if (!grid->opaque[index].empty()) {
            //                printf("1 ");
            //                ans++;
            //            } else {
            //                printf("0 ");
            //            }
            //        }
            //        printf("\n");
            //    }
            //    printf("\n");
            //}
            //printf("ans: %d", ans);
        } else
            grid = nullptr;
    }

    ~RayTracer() {}

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit);

    SceneParser *scene;
    int max_bounces;
    float min_weight;
    Grid *grid;

};

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit) {
    //grid shading
    if(nx != 0){
        if (grid->intersect(ray, hit, tmin)) {
            Vec3f color(0.0, 0.0, 0.0);
            Vec3f pos = hit.getIntersectionPoint();
            color += scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
            for (int k = 0; k < scene->getNumLights(); ++k) {
                Light *light = scene->getLight(k);
                Vec3f l, lightColor;
                float dis;
                light->getIllumination(pos, l, lightColor, dis);
                color += hit.getMaterial()->Shade(ray, hit, l, lightColor);
            }
            return color;
        } else {
            return scene->getBackgroundColor();
        }
    }


    //ray trace shading
    Group *group = scene->getGroup();
    if (group->intersect(ray, hit, tmin)) {
        if (bounces == 0) {
            RayTree::SetMainSegment(ray, tmin, hit.getT());    //main---------------------------------
        }
        Vec3f color(0.0, 0.0, 0.0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();
        color += scene->getAmbientLight() * material->getDiffuseColor();

        for (int k = 0; k < scene->getNumLights(); ++k) {
            Light *light = scene->getLight(k);
            Vec3f l, lightColor;
            float dis;
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


