#ifndef __DUNGEONS_CURSES_MAP_H__
#define __DUNGEONS_CURSES_MAP_H__

#include "fractal_heightmap.h"
#include "ui.h"

struct d_pos {
	int x, y;
};

struct d_size {
	int width, height;
};

struct d_area {
	struct d_pos pos;
	struct d_size size;
};

void d_curses_map_draw (struct d_area *area, struct d_ui_viewpoint *vp,
						int zoom, struct d_heightmap *hm);

#endif
