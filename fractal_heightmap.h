/*
 Written by: Paul E. Martz
 Copyright 1997 by Paul E. Martz, all right reserved
 Non-commercial use by individuals is permitted.
*/
#ifndef __DUNGEONS_FRACTAL_HEIGHTMAP_H__
#define __DUNGEONS_FRACTAL_HEIGHTMAP_H__

struct d_heightmap {
	int width;
	int height;
	float *array;
};

void d_fractal_heightmap_generate (struct d_heightmap *hm, int seed, float heightScale, float h);
struct d_heightmap* d_fractal_heightmap_new (int size);
void d_fractal_heightmap_destroy (struct d_heightmap *hm);

float d_fractal_heightmap_get (struct d_heightmap *hm, int x, int y);

#endif