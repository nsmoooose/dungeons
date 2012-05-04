#ifndef __DUNGEONS_WORLD_H__
#define __DUNGEONS_WORLD_H__

#include "heightmap.h"

struct d_pos {
	short x, y, z;
};

struct d_tile {
	struct d_pos pos;
	/*
	   struct d_list* objects;
	   shortcuts for adjascent tiles?
	*/
};

struct d_world {
	int width;
	int height;
	int depth;
	int ocean;

	struct d_heightmap *hm;
	struct d_tile **tiles;
};

struct d_tile *d_world_tile_get (struct d_world *world, int x, int y, int z, int create);

#endif
