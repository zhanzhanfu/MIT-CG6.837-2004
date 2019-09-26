#include "grid.h"

Grid::Grid(BoundingBox *bb, int nx, int ny, int nz) {
	boundingBox = bb;
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
	this->sum = nx * ny * nz;
	is_voxel_opaque.assign(sum, false);
	object3Ds.resize(sum);
}
Grid::~Grid() {

}

bool Grid::intersect(const Ray &r, Hit &h, float tmin){
	
	bool flag = false;
	//MarchingInfo mi;
	//initializeRayMarch(mi, r, tmin);

	//Vec3f min = boundingBox->getMin();
	//Vec3f max = boundingBox->getMax();
	//Vec3f size = max - min;
	//float grid_x = size.x() / nx;
	//float grid_y = size.y() / ny;
	//float grid_z = size.z() / nz;

	//if (mi.tmin < h.getT()) {
	//	while (mi.i < nx && mi.j < ny && mi.k < nz &&
	//		mi.i >= 0 && mi.j >= 0 && mi.k >= 0) {
	//		if (is_voxel_opaque[(mi.i * ny + mi.j) * nz + mi.k]) {
	//			if (object3Ds[(mi.i * ny + mi.j) * nz + mi.k]->material == NULL) {
	//				object3Ds[(mi.i * ny + mi.j) * nz + mi.k]->material = material;
	//			}
	//			/*Vec3f v1(mi.i * grid_x, mi.j * grid_y, mi.k * grid_z);
	//			Vec3f v2((mi.i + 1) * grid_x, mi.j * grid_y, mi.k * grid_z);
	//			Vec3f v3((mi.i + 1) * grid_x, (mi.j + 1) * grid_y, mi.k * grid_z);
	//			Vec3f v4(mi.i * grid_x, (mi.j + 1) * grid_y, mi.k * grid_z);
	//			Vec3f v5(mi.i * grid_x, mi.j * grid_y, (mi.k + 1) * grid_z);
	//			Vec3f v6((mi.i + 1) * grid_x, mi.j * grid_y, (mi.k + 1) * grid_z);
	//			Vec3f v7((mi.i + 1) * grid_x, (mi.j + 1) * grid_y, (mi.k + 1) * grid_z);
	//			Vec3f v8(mi.i * grid_x, (mi.j + 1) * grid_y, (mi.k + 1) * grid_z);
	//			RayTree::AddHitCellFace(v1, v4, v3, v2, Vec3f(0,0,-1), material);
	//			RayTree::AddHitCellFace(v1, v2, v6, v5, Vec3f(0, -1, 0), material);
	//			RayTree::AddHitCellFace(v1, v5, v8, v4, Vec3f(-1, 0, 0), material);
	//			RayTree::AddHitCellFace(v2, v3, v7, v6, Vec3f(1, 0, 0), material);
	//			RayTree::AddHitCellFace(v3, v4, v8, v7, Vec3f(0, 1, 0), material);
	//			RayTree::AddHitCellFace(v5, v6, v7, v8, Vec3f(0, 0, 1), material);*/
	//			
	//			h.set(mi.tmin, object3Ds[(mi.i * ny + mi.j) * nz + mi.k]->material, mi.normal, r);
	//			//h.set(mi.tmin, material, mi.normal, r);
	//			flag = true;
	//			break;
	//		}
	//		mi.nextCell();
	//	}
	//}
	
	return flag;
}

void Grid::paint() {
	material->glSetMaterial();
	Vec3f min = boundingBox->getMin();
	Vec3f max = boundingBox->getMax();
	Vec3f size = max - min;
	float grid_x = size.x() / nx;
	float grid_y = size.y() / ny;
	float grid_z = size.z() / nz;
	glPushMatrix();
	glTranslatef(min.x(), min.y(), min.z());
	for (int p=0;p<sum;p++)
	{
		// render 6 quads for each occupied grid cell
		// or to be more efficient, only draw the faces between occupied and unoccupied cells???
		if (is_voxel_opaque[p]) {
			//int k = p % nz;               //why 
			//int j = ((p - k) / nz) % ny;
			//int i = (((p - k) / nz) - j) / ny;
			int k = p / (nx * ny);
			int j = p % (nx * ny) / ny;
			int i = p % (nx * ny) % ny;
			glBegin(GL_QUADS);
			//Make sure you have specified the correct normals
			glVertex3f(i*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, k*grid_z);
			glVertex3f(i*grid_x, j*grid_y, k*grid_z);

			glVertex3f(i*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f(i*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glVertex3f(i*grid_x, (j + 1)*grid_y, k*grid_z);
			glVertex3f(i*grid_x, j*grid_y, k*grid_z);

			glVertex3f(i*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, k*grid_z);
			glVertex3f(i*grid_x, (j + 1)*grid_y, k*grid_z);

			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, k*grid_z);
			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, k*grid_z);

			glVertex3f(i*grid_x, j*grid_y, k*grid_z);
			glVertex3f(i*grid_x, (j + 1)*grid_y, k*grid_z);
			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, k*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, k*grid_z);

			glVertex3f(i*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, j*grid_y, (k + 1)*grid_z);
			glVertex3f((i + 1)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glVertex3f(i*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
			glEnd();
		}
	}
	glPopMatrix();

}

BoundingBox* Grid::getBoundingBox() {
	return boundingBox;
}

int Grid::getnx()
{
	return nx;
}

int Grid::getny()
{
	return ny;
}

int Grid::getnz()
{
	return nz;
}




void Grid::addObject(int i, bool v, Object3D *obj) {
	is_voxel_opaque[i] = v;
	object3Ds[i] = obj;
}

void Grid::setMaterial(Material *m) {
	material = m;
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const {
	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();
	Vec3f min = boundingBox->getMin();
	Vec3f max = boundingBox->getMax();
	float t_near = tmin;
	float t_far = INFINITY;
	Vec3f normal(0.0f, 0.0f, 0.0f);

	assert(t_near >= 0);

	float dirx = direction.x();
	float t1_x = (min.x() - origin.x()) / dirx;
	float t2_x = (max.x() - origin.x()) / dirx;
	if (t1_x > t2_x) {
		float temp = t1_x;
		t1_x = t2_x;
		t2_x = temp;
	}
	
	float diry = direction.y();
	float t1_y = (min.y() - origin.y()) / diry;
	float t2_y = (max.y() - origin.y()) / diry;
	if (t1_y > t2_y) {
		float temp = t1_y;
		t1_y = t2_y;
		t2_y = temp;
	}

	float dirz = direction.z();
	float t1_z = (min.z() - origin.z()) / dirz;
	float t2_z = (max.z() - origin.z()) / dirz;
	if (t1_z > t2_z) {
		float temp = t1_z;
		t1_z = t2_z;
		t2_z = temp;
	}

	
	t_near = max3(t1_x, t1_y, t1_z);
	t_far = min3(t2_x, t2_y, t2_z);

	if (t_near > t_far) return;
	
	mi.sign_x = (dirx > 0) ? 1 : -1;
	mi.sign_y = (diry > 0) ? 1 : -1;
	mi.sign_z = (dirz > 0) ? 1 : -1;

	if (t_near == t1_x)
		normal += Vec3f(-1.0f, 0.0f, 0.0f) * mi.sign_x;
	if (t_near == t1_y)
		normal += Vec3f(0.0f, -1.0f, 0.0f) * mi.sign_y;
	if (t_near == t1_z)
		normal += Vec3f(0.0f, 0.0f, -1.0f) * mi.sign_z;
	normal.Normalize();

	
	Vec3f size = max - min;
	float grid_x = size.x() / nx;
	float grid_y = size.y() / ny;
	float grid_z = size.z() / nz;
	
	mi.tmin = t_near;
	mi.dt_x = fabs(grid_x / dirx);
	mi.dt_y = fabs(grid_y / dirx);
	mi.dt_z = fabs(grid_z / dirz);

	if (t1_x > -INFINITY) {
		while (t1_x<t_near)
		{
			t1_x += mi.dt_x;
		}
		mi.tnext_x = t1_x;
	}
	if (t1_y > -INFINITY) {
		while (t1_y<t_near)
		{
			t1_y += mi.dt_y;
		}
		mi.tnext_y = t1_y;
	}
	if (t1_z > -INFINITY) {
		while (t1_z<t_near)
		{
			t1_z += mi.dt_z;
		}
		mi.tnext_z = t1_z;
	}

	Vec3f rp = origin + direction * t_near - min;
	rp.Divide(grid_x, grid_y, grid_z);
	mi.i = (int)rp.x(); 
	if (mi.sign_x < 0 && mi.i == nx) mi.i--;
	mi.j = (int)rp.y();
	if (mi.sign_y < 0 && mi.j == ny) mi.j--;
	mi.k = (int)rp.z();
	if (mi.sign_z < 0 && mi.k == nz) mi.k--;
	mi.normal = normal;
}