#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "hit.h"
#include "group.h"
#include "material.h"
#include "light.h"
#include "glCanvas.h"


char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool Blinn = true;
bool gui = false;
int theta_steps = 10;
int phi_steps = 5;
bool gouraud = false;


SceneParser *sp;
Group *group;
Camera *camera;
Image *image;
Image *depth_image;
Image *normal_image;
GLCanvas canvas;
void render() {
	sp = new SceneParser(input_file);
	group = sp->getGroup();
	camera = sp->getCamera();
	image = new Image(width, height);
	image->SetAllPixels(sp->getBackgroundColor());
	depth_image = new Image(width, height);
	depth_image->SetAllPixels(Vec3f(0, 0, 0));
	normal_image = new Image(width, height);
	normal_image->SetAllPixels(Vec3f(0, 0, 0));
	int numLights = sp->getNumLights();
	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = (float)i / width;
			float y = (float)j / height;
			
			Ray ray = camera->generateRay(Vec2f(x, y));
			Hit hit(INT_MAX, NULL,Vec3f(0,0,0));
			float tmin = camera->getTMin();
			
			if (group->intersect(ray, hit, tmin)) {
				Vec3f normal = hit.getNormal();
				Vec3f color = sp->getAmbientLight()*hit.getMaterial()->getDiffuseColor();
				for (int k = 0; k < numLights; k++)
				{
					Light *light = sp->getLight(k);
					Vec3f point = ray.pointAtParameter(hit.getT());
					Vec3f dirToLight;
					Vec3f colorToLight;
					float distanceToLight;
					light->getIllumination(point, dirToLight, colorToLight, distanceToLight);
					color += hit.getMaterial()->Shade(ray, hit, dirToLight, colorToLight);
				}
				image->SetPixel(i, j, color);
				float t = hit.getT();
				if (t < depth_min)
					t = depth_min;
				if (t > depth_max)
					t = depth_max;
				float d_c = (depth_max - t) / (depth_max - depth_min);
				depth_image->SetPixel(i, j, Vec3f(d_c, d_c, d_c));
				normal_image->SetPixel(i, j, Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));
			}				
		}
	}
	if (output_file != NULL) {
		image->SaveTGA(output_file);
	}
	if (depth_file != NULL)
	{
		depth_image->SaveTGA(depth_file);
	}
	if (normal_file != NULL)
	{
		normal_image->SaveTGA(normal_file);
	}
}

void glrender() {
	sp = new SceneParser(input_file);
	canvas.initialize(sp, render);
}
//raytracer -input scene1_01.txt -size 200 200 -output output1_01.tga -depth 9 10 depth1_01.tga
int main(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			theta_steps = atof(argv[i]);
			i++; assert(i < argc);
			phi_steps = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			gouraud = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	glutInit(&argc, argv);
	if (gui) {
		glrender();
	}
	else {
		render();
	}
	return 0;
}