#include "filter.h"

const float e = 2.718281828;

Vec3f Filter::getColor(int i, int j, Film *film) {
    Vec3f color(0, 0, 0);
    float weight = 0;
    int support_radius = getSupportRadius();
    int width = film->getWidth();
    int height = film->getHeight();
    for (int ii = i - support_radius; ii <= i + support_radius; ++ii) {
        for (int jj = j - support_radius; jj <= j + support_radius; ++jj) {
            if (ii < 0 || ii >= width || jj < 0 || jj >= height) continue;
            for (int nn = 0; nn < film->getNumSamples(); ++nn) {
                Sample s = film->getSample(ii, jj, nn);
                float x = ii + s.getPosition().x() - (i + 0.5);
                float y = jj + s.getPosition().y() - (j + 0.5);
                float w = getWeight(x, y);
                color += s.getColor() * w;
                weight += w;
            }
        }
    }
    return color * (1.0f / weight);
}


float BoxFilter::getWeight(float x, float y) {
    if (fabs(x) > radius || fabs(y) > radius)
        return 0;
    return 1;
}

int BoxFilter::getSupportRadius() {
    return radius + 0.4999;
}

float TentFilter::getWeight(float x, float y) {
    float d = sqrt(x * x + y * y);
    if (d > radius)
        return 0;
    return 1 - d / radius;
}

int TentFilter::getSupportRadius() {
    return radius + 0.4999;
}


float GaussianFilter::getWeight(float x, float y) {
    float d2 = x * x + y * y;
    float d = sqrt(d2);
    float sigma = 1.0f * radius;
    //if (d > 2 * sigma)
    //    return 0;
    float exp = -1 * d2 / (2 * sigma * sigma);
    return pow(e, exp);
}

int GaussianFilter::getSupportRadius() {
    return 2 * radius;
}


