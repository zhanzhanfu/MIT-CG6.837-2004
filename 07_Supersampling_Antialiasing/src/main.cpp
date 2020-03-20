#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "rayTracer.h"
#include "glCanvas.h"
#include "raytracing_stats.h"
#include "sampler.h"
#include "filter.h"

#include <cstring>
#include <iostream>
#include <GL/freeglut.h>

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
bool visualize_grid = false;
int nx = 0, ny = 0, nz = 0;
bool stats = false;
bool random_samples = false;
bool uniform_samples = false;
bool jittered_samples = false;
int num_samples = 1;
char *render_samples_file = nullptr;
char *render_filter_file = nullptr;
int zoom_factor = 0;
bool box_filter = false;
bool tent_filter = false;
bool gaussian_filter = false;
float filter_radius = 0;

SceneParser *scene;

void argParser(int argc, char **argv);

void renderFunction() {
    Camera *camera = scene->getCamera();
    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());

    RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    if (nx != 0)
        RayTracingStats::Initialize(width, height, rayTracer.getGrid()->getBoundingBox(), nx, ny, nz);
    else
        RayTracingStats::Initialize(width, height, nullptr, 0, 0, 0);

    Film *film = new Film(width, height, num_samples);
    Sampler *sampler = nullptr;
    if (random_samples)
        sampler = new RandomSampler(num_samples);
    if (jittered_samples)
        sampler = new JitteredSampler(num_samples);
    if (uniform_samples || !sampler)
        sampler = new UniformSampler(num_samples);

    Filter *filter = nullptr;
    if (box_filter)
        filter = new BoxFilter(filter_radius);
    if (tent_filter)
        filter = new TentFilter(filter_radius);
    if (gaussian_filter)
        filter = new GaussianFilter(filter_radius);

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int n = 0; n < num_samples; ++n) {
                //RayTracingStats::IncrementNumNonShadowRays();//----------------------------------------------
                Vec2f offset = sampler->getSamplePosition(n);
                float x = float(i + offset.x()) / float(width);
                float y = float(j + offset.y()) / float(height);
                Ray ray = camera->generateRay(Vec2f(x, y));
                float tmin = 0.0001f;
                Hit hit(INFINITY);
                Vec3f c = rayTracer.TraceRay(ray, tmin, 0, 1.0, hit);
                film->setSample(i, j, n, offset, c);
            }
        }
    }
    if (output_file) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if(filter)
                    image.SetPixel(i, j, filter->getColor(i, j, film));
                else
                    image.SetPixel(i, j, film->getSample(i, j, 0).getColor());
            }
        }
        image.SaveTGA(output_file);
    }
    if (render_samples_file) {
        film->renderSamples(render_samples_file, zoom_factor);
    }
    if (render_filter_file) {
        film->renderFilter(render_filter_file, zoom_factor, filter);
    }
    if (stats) {
        RayTracingStats::PrintStatistics();
    }
}

void traceRayFunction(float x, float y) {
    //cout << x << "" << y << endl;
    Ray ray = scene->getCamera()->generateRay(Vec2f(x, y));
    RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    float tmin = 0.0001f;
    Hit hit(INFINITY);
    rayTracer.TraceRay(ray, tmin, 0, 1.0, hit);
    Hit hit2(INFINITY);
    rayTracer.getGrid()->intersect(ray, hit2, tmin);
}

int main(int argc, char **argv) {
// sample command line:
    // -input scene4_03_mirrored_floor.txt -size 200 200 -output output4_03.tga -shadows -bounces 1 -weight 0.01
    argParser(argc, argv);
    scene = new SceneParser(input_file);

    //gui
    if (gui) {
        Grid *grid = nullptr;
        if (nx != 0) {
            grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
            scene->getGroup()->insertIntoGrid(grid, nullptr);
        }
        glutInit(&argc, argv);
        GLCanvas glCanvas;
        glCanvas.initialize(scene, renderFunction, traceRayFunction, grid, visualize_grid);
        return 0;
    } else {
        renderFunction();
    }

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
        } else if (!strcmp(argv[i], "-grid")) {
            i++;
            assert(i < argc);
            nx = atoi(argv[i]);
            i++;
            assert(i < argc);
            ny = atoi(argv[i]);
            i++;
            assert(i < argc);
            nz = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-visualize_grid")) {
            visualize_grid = true;
        } else if (!strcmp(argv[i], "-stats")) {
            stats = true;
        } else if (!strcmp(argv[i], "-random_samples")) {
            random_samples = true;
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-uniform_samples")) {
            uniform_samples = true;
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-jittered_samples")) {
            jittered_samples = true;
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-box_filter")) {
            box_filter = true;
            i++;
            assert(i < argc);
            filter_radius = atof(argv[i]);
        } else if (!strcmp(argv[i], "-tent_filter")) {
            tent_filter = true;
            i++;
            assert(i < argc);
            filter_radius = atof(argv[i]);
        } else if (!strcmp(argv[i], "-gaussian_filter")) {
            gaussian_filter = true;
            i++;
            assert(i < argc);
            filter_radius = atof(argv[i]);
        } else if (!strcmp(argv[i], "-render_samples")) {
            i++;
            assert(i < argc);
            render_samples_file = argv[i];
            i++;
            assert(i < argc);
            zoom_factor = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-render_filter")) {
            i++;
            assert(i < argc);
            render_filter_file = argv[i];
            i++;
            assert(i < argc);
            zoom_factor = atoi(argv[i]);
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}

