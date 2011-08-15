#ifndef __DUNGEONS_UI_H__
#define __DUNGEONS_UI_H__

enum d_ui_map_mode {
	/* Draws like a regular map where we can see information from all heights. */
	d_map,
	/* Draw a slice from the current viewpoint. You see through the earth and. */
	d_level,
};

struct d_ui_viewpoint {
	int x, y, z;
};

#endif
