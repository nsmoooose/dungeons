#include <stdio.h>

#include "heightmap.h"
#include "error.h"
#include "memory.h"
#include "world.h"

struct d_tile *
d_world_tile_new () {
	struct d_tile *tile = d_calloc (1, sizeof (struct d_tile));
	return tile;
}

void
d_world_tile_free (struct d_tile *tile) {
	/* d_free (tile); */
}

struct d_tile *
d_world_tile_get (struct d_world *world, int x, int y, int z, int create) {
	if (x < 0 || y < 0 || z < 0 ||
	    x > world->width || y > world->height || z > world->depth) {
		d_bug ("position out of range");
	}
	return 0;
}

void
d_world_free (struct d_world *world) {
	d_heightmap_destroy (world->hm);
	d_free (world);
}
