#ifndef __DUNGEONS_CURSES_MAP_H__
#define __DUNGEONS_CURSES_MAP_H__

#include "game.h"
#include "heightmap.h"
#include "ui.h"

void d_map_coord_to_screen (struct d_ui_area *area, struct d_viewpoint *vp,
							int zoom, struct d_ui_pos *coord,
							struct d_ui_pos *screen_pos);
void d_map_coord_to_real (struct d_ui_area *area, struct d_viewpoint *vp,
					 int zoom, struct d_ui_pos *screen_coords, struct d_ui_pos *pos);

void d_map_draw (struct d_ui_area *area, struct d_viewpoint *vp,
				 int zoom, struct d_heightmap *hm);

#endif
