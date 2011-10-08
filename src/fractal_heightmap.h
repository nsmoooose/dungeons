/*
 Written by: Paul E. Martz
 Copyright 1997 by Paul E. Martz, all right reserved
 Non-commercial use by individuals is permitted.
*/
#ifndef __DUNGEONS_FRACTAL_HEIGHTMAP_H__
#define __DUNGEONS_FRACTAL_HEIGHTMAP_H__

#include "heightmap.h"

void d_fractal_heightmap_generate (struct d_heightmap *hm, int seed, float heightScale, float h);

#endif
