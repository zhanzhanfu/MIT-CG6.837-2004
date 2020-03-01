#include "scene_parser.h"
#include "camera.h"
#include "object3d.h"
#include "image.h"
#include "light.h"
#include "rayTracer.h"
#include <cstring>
#include <iostream>


char *input_file = nullptr;
int width = 100;
int height = 100;
char *output_file = nullptr;
float depth_min = 0;
float depth_max = 1;
char *depth_file = nullptr;
char *normal_file = nullptr;
bool shade_back = false;
bool shadows = false;
int max_bounces = 0;
float cutoff_weight = 1.0;
int nx = 0;
int ny = 0;
int nz = 0;
bool isGrid = false;

void argParser(int argc, char **argv);

int main(int argc, char **argv) {

    // sample command line:
    // -input scene5_01_sphere.txt -size 200 200 -output output5_01a.tga -grid 3 3 3
    argParser(argc, argv);

    SceneParser scene(input_file);
    Camera *camera = scene.getCamera();

    Image *image = new Image(width, height);
    image->SetAllPixels(scene.getBackgroundColor());

    Vec3f resolution(nx, ny, nz);
    RayTracer rayTracer(&scene, max_bounces, cutoff_weight, shadows, shade_back, isGrid, resolution);
    //for (int i = 0; i < width; ++i) {
    //    for (int j = 0; j < height; ++j) {
    //        float x = float(i) / float(width);
    //        float y = float(j) / float(height);
    //        Ray ray = camera->generateRay(Vec2f(x, y));
    //        float tmin = 0.001f;
    //        Vec3f color = rayTracer.traceRay(ray, tmin, 0, 1.0, 1.0);
    //        image.SetPixel(i, j, color);
    //    }
    //}
//
    if (output_file != nullptr)
        image->SaveTGA(output_file);

    return 0;
}

void argParser(int argc, char **argv) {
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
        } else if (!strcmp(argv[i], "-normals")) {
            i++;
            assert(i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i], "-shade_back")) {
            shade_back = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++;
            assert(i < argc);
            max_bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++;
            assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else if (!strcmp(argv[i], "-grid")) {
            isGrid = true;
            i++;
            assert(i < argc);
            nx = atoi(argv[i]);
            i++;
            assert(i < argc);
            ny = atoi(argv[i]);
            i++;
            assert(i < argc);
            nz = atoi(argv[i]);
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}