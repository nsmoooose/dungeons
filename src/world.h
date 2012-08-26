#ifndef __DUNGEONS_WORLD_H__
#define __DUNGEONS_WORLD_H__

#include "heightmap.h"
#include "octree.h"

struct d_world {
	int width;
	int height;
	int depth;
	int ocean;

	struct d_heightmap *hm;
	struct d_octree *tree;
};

struct d_tile *d_world_tile_new ();
void d_world_tile_free (struct d_tile *tile);
struct d_tile *d_world_tile_get (struct d_world *world, int x, int y, int z, int create);
void d_world_free (struct d_world *world);

#endif
