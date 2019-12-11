#include "scene_parser.h"
#include "camera.h"
#include "object3d.h"
#include "image.h"
#include "light.h"
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

void argParser(int argc, char **argv);

void render();

int main(int argc, char **argv) {

    // sample command line:
    // raytracer -input scene3_01_cube_orthographic.txt -size 200 200 -output output3_01.tga -gui
    // raytracer -input scene3_02_cube_perspective.txt -size 200 200 -output output3_02.tga -gui
    argParser(argc, argv);

    SceneParser scene(input_file);
    Camera *camera = scene.getCamera();
    Object3D *group = scene.getGroup();
    Vec3f ambientColor = scene.getAmbientLight();

    Image image(width, height);
    image.SetAllPixels(scene.getBackgroundColor());
    Image depthImage(width, height);
    depthImage.SetAllPixels(Vec3f(0.0, 0.0, 0.0));
    Image normalImage(width, height);
    normalImage.SetAllPixels(Vec3f(0.0, 0.0, 0.0));

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = float(i) / float(width);
            float y = float(j) / float_t(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            Hit hit(INFINITY, nullptr, Vec3f(0.0, 0.0, 0.0));
            bool flag = group->intersect(ray, hit, camera->getTMin());
            if (flag) {
                //shadow_back
                Vec3f normal = hit.getNormal();
                Vec3f rd = ray.getDirection();
                if (normal.Dot3(rd) > 0) {
                    if (shade_back) {
                        normal = -1 * normal;
                    } else {
                        //for scene2_07 black triangle
                        image.SetPixel(i, j, Vec3f(0.0, 0.0, 0.0));
                        continue;
                    }
                }
                //shader
                Vec3f color(0.0, 0.0, 0.0);
                Material *material = hit.getMaterial();
                Vec3f diffuseColor = material->getDiffuseColor();
                Vec3f ambient = diffuseColor * ambientColor;
                color += ambient;
                for (int k = 0; k < scene.getNumLights(); ++k) {
                    Light *light = scene.getLight(k);
                    Vec3f pos = hit.getIntersectionPoint();
                    Vec3f l, lightColor;
                    float d; //not use
                    light->getIllumination(pos, l, lightColor, d);
                    Vec3f c = material->Shade(ray, hit, l, lightColor);
                    color += c;
                }
                image.SetPixel(i, j, color);
                normalImage.SetPixel(i, j, Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));
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
    if (normal_file != nullptr)
        normalImage.SaveTGA(normal_file);
    return 0;
}

void render() {

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
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}