#pragma once

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "object3d.h"
#include "camera.h"
#include "light.h"

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back) :
            scene(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight), shadows(shadows),
            shade_back(shade_back) {}

    ~RayTracer() {}

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction);

    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);

    bool
    transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted);

    Vec3f shade(Material *material, Vec3f &normal, Vec3f &v, Vec3f &l, Vec3f &lightColor);

    SceneParser *scene;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
    bool shade_back;
};

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction) {
    Group *group = scene->getGroup();
    //shader
    Hit hit(INFINITY, nullptr, Vec3f());
    if (group->intersect(ray, hit, tmin)) {
        Vec3f color(0.0, 0.0, 0.0);
        Vec3f normal = hit.getNormal();
        Vec3f rd = ray.getDirection();
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();
        Vec3f v = -1 * rd;
        color += scene->getAmbientLight() * material->getDiffuseColor();

        bool outside = true;
        if (normal.Dot3(rd) > 0 && shade_back) {
            normal = -1 * normal;
            outside = false;
        }

        for (int k = 0; k < scene->getNumLights(); ++k) {
            Light *light = scene->getLight(k);
            Vec3f l, lightColor;
            float dis; //not be used
            light->getIllumination(pos, l, lightColor, dis);
            //shadows
            if (shadows) {
                Ray ray2(pos, l);
                Hit hit2(dis, nullptr, Vec3f());
                if (!group->intersectShadowRay(ray2, hit2, tmin))
                    color += shade(material, normal, v, l, lightColor);

            } else
                color += shade(material, normal, v, l, lightColor);
        }
        //reflective
        Vec3f k_ref = material->getReflectiveColor();
        if (bounces < max_bounces && weight > cutoff_weight && k_ref.Length() > 0) {
            Vec3f rd_ref = mirrorDirection(normal, rd);
            Ray ray_ref(pos, rd_ref);
            Vec3f color_ref = traceRay(ray_ref, tmin, bounces + 1, weight * k_ref.Length(), indexOfRefraction);
            color += k_ref * color_ref;
        }
        //refraction transparent
        Vec3f k_trans = material->getTransparentColor();
        if (bounces < max_bounces && weight > cutoff_weight && k_trans.Length() > 0) {
            //shade_back
            float index_i;
            float index_t;
            if (outside) {
                index_i = 1.0;
                index_t = material->getIndexOfRefraction();
            } else {
                index_i = material->getIndexOfRefraction();
                index_t = 1.0;
            }
            Vec3f rd_trans;
            if (transmittedDirection(normal, rd, index_i, index_t, rd_trans)) {
                Ray ray_trans(pos, rd_trans);
                Vec3f color_trans = traceRay(ray_trans, tmin, bounces + 1, weight * k_trans.Length(),
                                             indexOfRefraction);
                color += k_trans * color_trans;
            }
        }
        //ambient
        return color;
    } else {
        return scene->getBackgroundColor();
    }
}


Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) {
    Vec3f out = incoming - 2.0f * normal.Dot3(incoming) * normal;
    out.Normalize();
    return out;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t,
                                     Vec3f &transmitted) {
    bool flag = false;
    float nr = index_i / index_t;
    float d = -1 * normal.Dot3(incoming);
    float t = 1 - nr * nr * (1 - d * d);
    if (t > 0) {
        transmitted = (nr * d - sqrt(t)) * normal + nr * incoming;
        transmitted.Normalize();
        flag = true;
    }
    return flag;
}

Vec3f RayTracer::shade(Material *material, Vec3f &normal, Vec3f &v, Vec3f &l, Vec3f &lightColor) {
    Vec3f diffuseColor = material->getDiffuseColor();
    Vec3f specularColor = material->getSpecularColor();
    float exponent = material->getExponent();
    //diff
    float diff = std::max(normal.Dot3(l), 0.0f);
    Vec3f diffuse = diff * diffuseColor * lightColor;
    //spec
    Vec3f h = v + l;
    h.Normalize();
    float spec = pow(max(normal.Dot3(h), 0.0f), exponent);
    Vec3f specular = spec * specularColor * lightColor;
    //ans
    Vec3f color = diffuse + specular;
    return color;
}

