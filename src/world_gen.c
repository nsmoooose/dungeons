#include <math.h>

#include "fractal_heightmap.h"
#include "memory.h"
#include "octree.h"
#include "world_gen.h"

struct d_world *
d_world_generate (struct d_world_gen_params *params) {
	struct d_world *world = d_calloc (1, sizeof (struct d_world));

	/* Generate the heightmap using a fractal algorithm.
	   This heightmap is then used to build the tiles needed
	   for the surface. */
	world->hm = d_heightmap_new (params->size);
	d_fractal_heightmap_generate (world->hm, 123, 10000.f, 0.9f);

	/* Retreive highest and lowest point of the world. */
	int high, low;
	d_heightmap_get_highlow (world->hm, &high, &low);

	/* Create a world that is capable of low - 50 -> high + 50 tiles. */
	world->width = params->size;
	world->height = params->size;
	world->depth = fabs (low - 50) + fabs (high + 50);
	world->ocean = fabs (low - 50);

	world->tree = d_octree_new (5, 1000);
	for (int x=0;x<world->width;++x) {
		for (int y=0;y<world->height;++y) {
			int z = d_heightmap_get (world->hm, x, y);
			d_octree_insert (world->tree, x, y, z, 0);
		}
	}

	return world;
}
