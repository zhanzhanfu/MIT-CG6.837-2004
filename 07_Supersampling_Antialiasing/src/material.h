#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include "perlin_noise.h"
#include "matrix.h"
#include <algorithm>

class Material {

public:

    Material(const Vec3f &d_color) : diffuseColor(d_color) {}

    virtual ~Material() {};

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const = 0;

    virtual void glSetMaterial() const = 0;

    virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const = 0;

    virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const = 0;

    virtual Vec3f getDiffuseColor() const { return diffuseColor; }

protected:
    Vec3f diffuseColor;
};

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &d_color) : Material(d_color), exponent(0), indexOfRefraction(0) {}

    PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e, const Vec3f &ref_color, const Vec3f &trans_color, float index) :
            Material(d_color), specularColor(s_color), exponent(e), reflectiveColor(ref_color), transparentColor(trans_color),
            indexOfRefraction(index) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const;

    virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const;

private:

    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
};

class Checkerboard : public Material {
public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2) : Material(Vec3f()), matrix(m), material1(mat1), material2(mat2) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

    bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

private:
    Material *material1;
    Material *material2;
    Matrix *matrix;
};

class Noise : public Material {
public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) : Material(Vec3f()), matrix(m), material1(mat1), material2(mat2),
                                                                    octaves(octaves) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

    bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

    Vec3f getDiffuseColor() const override;

private:
    Material *material1;
    Material *material2;
    Matrix *matrix;
    int octaves;
};

class Marble : public Material {
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) :
            Material(Vec3f()), matrix(m), material1(mat1), material2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

    bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

    Vec3f getDiffuseColor() const override;

private:
    Material *material1;
    Material *material2;
    Matrix *matrix;
    int octaves;
    float frequency;
    float amplitude;
};

class Wood : public Material {
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) :
            Material(Vec3f()), matrix(m), material1(mat1), material2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

    bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

    Vec3f getDiffuseColor() const override;

private:
    Material *material1;
    Material *material2;
    Matrix *matrix;
    int octaves;
    float frequency;
    float amplitude;
};

#endif