#ifndef __DUNGEONS_CURSES_MAP_H__
#define __DUNGEONS_CURSES_MAP_H__

#include "fractal_heightmap.h"
#include "ui.h"

void d_map_draw (struct d_ui_area *area, struct d_ui_viewpoint *vp,
				 int zoom, struct d_heightmap *hm);

#endif
