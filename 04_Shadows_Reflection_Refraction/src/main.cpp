#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "rayTracer.h"
#include "glCanvas.h"
#include <cstring>
#include <iostream>
#include <GL/freeglut.h>

SceneParser *scene;
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
bool gui = false;
int theta_steps = 0;
int phi_steps = 0;
bool gouraud = false;

void argParser(int argc, char **argv);

void renderFunction() {}

void traceRayFunction(float x, float y) {
    //cout << x << "" << y << endl;
    Ray ray = scene->getCamera()->generateRay(Vec2f(x, y));
    RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    float tmin = 0.001f;
    Hit hit(INFINITY);
    Vec3f color = rayTracer.traceRay(ray, tmin, 0, 1.0, hit);
}

int main(int argc, char **argv) {

    // sample command line:
    // -input scene4_03_mirrored_floor.txt -size 200 200 -output output4_03.tga -shadows -bounces 1 -weight 0.01
    argParser(argc, argv);

    scene = new SceneParser(input_file);
    Camera *camera = scene->getCamera();

    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());

    if (gui) {
        glutInit(&argc, argv);
        GLCanvas glCanvas;
        glCanvas.initialize(scene, renderFunction, traceRayFunction);
        return 0;
    }

    RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = float(i) / float(width);
            float y = float(j) / float(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            float tmin = 0.001f;
            Hit hit(INFINITY);
            Vec3f color = rayTracer.traceRay(ray, tmin, 0, 1.0, hit);
            image.SetPixel(i, j, color);
        }
    }

    if (output_file != nullptr)
        image.SaveTGA(output_file);

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
        } else if (!strcmp(argv[i], "-gui")) {
            gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++;
            assert(i < argc);
            theta_steps = atoi(argv[i]);
            i++;
            assert(i < argc);
            phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++;
            assert(i < argc);
            max_bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++;
            assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}