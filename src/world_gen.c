#include <math.h>

#include "fractal_heightmap.h"
#include "memory.h"
#include "object_tile.h"
#include "world_gen.h"

static struct d_octree *
d_world_gen_hm_to_tiles (struct d_heightmap *hm) {
	/* Retreive highest and lowest point of the world. */
	int high, low;
	d_heightmap_get_highlow (hm, &high, &low);

	struct d_point3 offset = { 0, 0, low };

	struct d_octree *tree = d_octree_new (8, hm->width);
	for (int x=0;x<=hm->width;++x) {
		for (int y=0;y<=hm->height;++y) {
			short z = d_heightmap_get (hm, x, y);
			/* TODO We should handle several different tile types like water and rock. */
			struct d_ob_type *type = d_ob_category_tiles.objects[0];
			struct d_ob_instance *inst = type->create (type, x, y, z);
			d_octree_insert (tree, offset.x + x, offset.y + y, offset.z + z, inst);
		}
	}
	return tree;
}

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

	/* int m = fmax (world->depth, fmax (world->width, world->height)); */
	world->tree = d_world_gen_hm_to_tiles (world->hm);

	return world;
}
