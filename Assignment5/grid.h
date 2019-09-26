#pragma once
#include "object3d.h"
#include "marchingInfo.h"
#include <vector>
#include "rayTree.h"

#define min3(a,b,c) (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c)))
#define max3(a,b,c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))

class Grid :public Object3D {
private:

	vector<Object3D*> object3Ds; //为了储存每个voxel的material
	bool visualize;
	int nx, ny, nz;
	int sum;
public:
	vector<bool> is_voxel_opaque;
	
	Grid(BoundingBox *bb, int nx, int ny, int nz);
	virtual ~Grid(); 
	virtual bool intersect(const Ray &r, Hit &h, float tmin) override;
	virtual void paint() override;
	virtual BoundingBox* getBoundingBox() override;

	int getnx();
	int getny();
	int getnz();
	void addObject(int i, bool v, Object3D *obj);
	void setMaterial(Material *m);
	void initializeRayMarch(MarchingInfo &mi,const Ray &r, float tmin) const;
};