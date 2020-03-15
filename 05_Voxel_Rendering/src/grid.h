#pragma once

#include "object3d.h"
#include <vector>
#include "marchingInfo.h"

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz) {
        boundingBox = bb;
        opaque.resize(nx * ny * nz);
        material = new PhongMaterial(Vec3f(1, 1, 1));
    }

    ~Grid() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin);

    virtual void paint() const;

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

    int nx, ny, nz;
    //std::vector<bool> opaque;
    std::vector<std::vector<Object3D*> > opaque;
};