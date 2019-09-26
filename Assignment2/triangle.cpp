#include "triangle.h"

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
	this->a = a;
	this->b = b;
	this->c = c;
	material = m;
	Vec3f::Cross3(normal, b - a, c - a);
	normal.Normalize();

}
Triangle::~Triangle() {

}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
	bool flag = false;
	float Beta, Gamma, t;
	float M;
	float A = a.x() - b.x();
	float B = a.y() - b.y();
	float C = a.z() - b.z();
	float D = a.x() - c.x();
	float E = a.y() - c.y();
	float F = a.z() - c.z();
	float G = r.getDirection().x();
	float H = r.getDirection().y();
	float I = r.getDirection().z();
	float J = a.x() - r.getOrigin().x();
	float K = a.y() - r.getOrigin().y();
	float L = a.z() - r.getOrigin().z();
	M = A * (E*I - F * H) - B * (D*I - F * G) + C * (D*H - E * G);
	Beta = (J*(E*I - F * H) - K * (D*I - F * G) + L * (D*H - E * G)) / M;
	Gamma = (A*(K*I - L * H) - B * (J*I - L * G) + C * (J*H - K * G)) / M;
	if (Beta > 0 && Gamma > 0 && (Beta + Gamma) < 1) {
		t = (A*(E*L - F * K) - B * (D*L - F * J) + C * (D*K - E * J)) / M;
		if (t > tmin && t < h.getT()) {
			h.set(t, material, normal, r);
			flag = true;
		}
	}
	return flag;
}