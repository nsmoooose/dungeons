#ifndef __DUNGEONS_WORLDGEN_H__
#define __DUNGEONS_WORLDGEN_H__

#include "world.h"

struct d_world_gen_params {
	int size;
};

struct d_world *d_world_generate (struct d_world_gen_params *params);

#endif
