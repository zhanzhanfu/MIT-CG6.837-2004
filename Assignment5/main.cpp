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
#include "rayTracer.h"
#include "grid.h"


char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;
bool Blinn = true;
bool gui = false;
int theta_steps = 10;
int phi_steps = 5;
bool gouraud = false;
bool shadows = false;
int max_bounces = 0;
float cutoff_weight = 0;
//bool useTransparentShadows = true;
int nx, ny, nz;
bool visualize_grid = false;

SceneParser *sp;
Group *group;
Camera *camera;
Image *image;
Image *depth_image;
Image *normal_image;
Grid *grid=NULL;

void render() {
	
	group = sp->getGroup();
	camera = sp->getCamera();
	image = new Image(width, height);
	//image->SetAllPixels(sp->getBackgroundColor());
	//depth_image = new Image(width, height);
	//depth_image->SetAllPixels(Vec3f(0, 0, 0));
	//normal_image = new Image(width, height);
	//normal_image->SetAllPixels(Vec3f(0, 0, 0));
	RayTracer rayTracer = RayTracer(sp, max_bounces, cutoff_weight, shadows, Vec3f(nx, ny, nz), visualize_grid);
	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = (float)i / width;
			float y = (float)j / height;
			Ray ray = camera->generateRay(Vec2f(x, y));
			//Material *material = sp->getMaterial(0);
			Hit hit(INT_MAX, NULL ,Vec3f(0,0,0));
			float tmin = camera->getTMin();
			
			Vec3f pixel = rayTracer.traceRay(ray, tmin, 1, 1.0f,
				1, hit);
			image->SetPixel(i, j, pixel);
			/*
			float t = hit.getT();
			if (t < depth_min)
				t = depth_min;
			if (t > depth_max)
				t = depth_max;
			float d_c = (depth_max - t) / (depth_max - depth_min);
			depth_image->SetPixel(i, j, Vec3f(d_c, d_c, d_c));
			Vec3f normal = hit.getNormal();

			normal_image->SetPixel(i, j, Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));				
			*/
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
void renderFunction() {

}
void traceRayFunction(float x, float y) {
	Ray ray = sp->getCamera()->generateRay(Vec2f(x,y));
	Hit hit(INT_MAX, NULL, Vec3f());
	float tmin = 0.01f;
	RayTracer rt(sp, max_bounces, cutoff_weight, shadows,Vec3f(nx,ny,nz),visualize_grid);
	Vec3f pixel = rt.traceRay(ray, tmin, 1, 1, 1, hit);
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
		else if (!strcmp(argv[i], "-shade_back")) {
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			theta_steps = atoi(argv[i]);
			i++; assert(i < argc);
			phi_steps = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-shadows")) {
			shadows = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++; assert(i < argc);
			max_bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++; assert(i < argc);
			cutoff_weight = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-grid")) {
			i++; assert(i < argc);
			nx = atoi(argv[i]);
			i++; assert(i < argc);
			ny = atoi(argv[i]);
			i++; assert(i < argc);
			nz = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-visualize_grid")) {
			visualize_grid = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	glutInit(&argc, argv);
	sp = new SceneParser(input_file);
	if (gui) {
		//RayTracer rayTracer = RayTracer(sp, max_bounces, cutoff_weight, shadows, Vec3f(nx, ny, nz), visualize_grid);
		//grid = rayTracer.getGrid();
		Group* group = sp->getGroup();
		grid = new Grid(group->getBoundingBox(), nx, ny, nz);
		grid->setMaterial(sp->getMaterial(0));
		group->setIsGrid(true);
		group->insertIntoGrid(grid, NULL);
		GLCanvas canvas;
		canvas.initialize(sp, renderFunction, traceRayFunction, grid, visualize_grid);
	}
	else {
		render();
	}
	return 0;
}