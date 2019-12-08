#include "scene_parser.h"
#include "camera.h"
#include "object3d.h"
#include "image.h"
#include <cstring>
#include <iostream>

int main(int argc, char **argv) {
    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;
// sample command line:
// raytracer -input scene1_01.txt -size 200 200 -output output1_01.tga -depth 9 10 depth1_01.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert(i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert(i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i], "-depth")) {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
    SceneParser scene(input_file);
    Camera *camera = scene.getCamera();
    Object3D *group = scene.getGroup();
    Image image(width, height);
    image.SetAllPixels(scene.getBackgroundColor());
    Image depthImage(width, height);
    depthImage.SetAllPixels(Vec3f(0.0, 0.0, 0.0));

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = float(i) / float(width);
            float y = float(j) / float_t(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            Hit hit(INFINITY, nullptr);
            bool flag = group->intersect(ray, hit, camera->getTMin());
            if (flag) {
                image.SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
                float t = hit.getT();
                if (t > depth_max) t = depth_max;
                if (t < depth_min) t = depth_min;
                t = (depth_max - t) / (depth_max - depth_min);
                depthImage.SetPixel(i, j, Vec3f(t, t, t));
            }
        }
    }
    if (output_file != nullptr)
        image.SaveTGA(output_file);
    if (depth_file != nullptr)
        depthImage.SaveTGA(depth_file);
    return 0;
}