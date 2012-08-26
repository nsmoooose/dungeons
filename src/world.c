#include <stdio.h>

#include "heightmap.h"
#include "memory.h"
#include "octree.h"
#include "world.h"

void
d_world_free (struct d_world *world) {
	d_heightmap_destroy (world->hm);
	d_octree_destroy (world->tree);
	d_free (world);
}
