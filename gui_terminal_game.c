#include <stdio.h>

#include "client.h"
#include "gui_terminal_game.h"
#include "gui_terminal_io.h"

struct d_client_context *d_context = 0;

static struct d_gui_terminal_game_view_point {
	int x, y, z;
} d_viewpoint;

void
d_gui_terminal_draw_terrain () {
	d_gui_terminal_set_color (d_green, d_light_green);
	d_gui_terminal_clear ();

	d_gui_terminal_printf_left (12, 7, ".");
}

void
d_gui_terminal_game_update (double now, double delta) {
}

void
d_gui_terminal_game_draw () {
	d_gui_terminal_draw_terrain ();

	d_gui_terminal_set_color (d_white, d_white);
	d_gui_terminal_box (1, 1, d_gui_terminal_size.width,
						d_gui_terminal_size.height, ' ');
	d_gui_terminal_set_color (d_black, d_green);
	d_gui_terminal_printf_center (d_gui_terminal_size.width / 2, 1,
								  "|======- D U N G E O N S -======|");

	d_gui_terminal_set_color (d_black, d_white);
	d_gui_terminal_printf_left (5, 1, " Grid x: 123, y: 221. above sea: 1333m ",
								d_viewpoint.x, d_viewpoint.y, d_viewpoint.z);
	d_gui_terminal_printf_right (d_gui_terminal_size.width - 5, 1,
								 " Tuesday 15 march year 723 ");

	d_gui_terminal_set_color (d_none, d_white);
}

void
d_gui_terminal_game_key (char key) {
	switch (key) {
	case 'q':
		d_quit = 1;
		break;
	case 'z':
		/* TODO Zoom in. */
		break;
	case 'Z':
		/* TODO Zoom out. */
		break;
	case '<':
		/* TODO Go down one level. */
		break;
	case '>':
		/* TODO go up one level. */
		break;
	case 'p':
		/* TODO pause the game. */
		break;
	default:
		printf ("Received key: %d\n", key);
	}
}
