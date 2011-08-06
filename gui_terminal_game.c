#include <stdio.h>

#include "client.h"
#include "fractal_heightmap.h"
#include "gui_terminal_game.h"
#include "gui_terminal_io.h"
#include "memory.h"

struct d_gui_terminal_game_view_point {
	int x, y, z;
};

/* static struct d_client_context *d_context = 0; */
static struct d_heightmap *d_terrain = 0;
static struct d_gui_terminal_game_view_point *d_viewpoint=0;

void
d_gui_terminal_draw_terrain () {
	if (!d_viewpoint || !d_terrain) {
		d_gui_terminal_set_color (d_green, d_light_green);
		d_gui_terminal_clear ();
		return;
	}

	for (int x=2;x<=d_gui_terminal_size.width-1;++x) {
		for (int y=2;y<=d_gui_terminal_size.height-1;++y) {
			int realx = d_viewpoint->x - d_gui_terminal_size.width / 2 + x;
			int realy = d_viewpoint->y - d_gui_terminal_size.height / 2 + y;

			float f = d_fractal_heightmap_get (d_terrain, realx, realy);
			if (f < d_viewpoint->z - 5.0) {
				/* earth. Should be black */
				d_gui_terminal_set_color (d_black, d_light_gray);
				d_gui_terminal_printf_left (x, y, " ");
			}
			else if (f > d_viewpoint->z + 5.0) {
				/* sky. should be blueish. */
				d_gui_terminal_set_color (d_cyan, d_light_green);
				d_gui_terminal_printf_left (x, y, " ");
			}
			else {
				/* On this level. Green as gras. */
				d_gui_terminal_set_color (d_green, d_light_green);
				d_gui_terminal_printf_left (x, y, " ");
			}
		}
	}
}

void
d_gui_terminal_game_update (double now, double delta) {
	if (!d_terrain) {
		int size = 2048;
		d_terrain = d_fractal_heightmap_new (size);
		d_fractal_heightmap_generate (d_terrain, 123, 10000.f, 0.7f);
		d_viewpoint = d_calloc (1, sizeof (struct d_gui_terminal_game_view_point));
		d_viewpoint->x = size / 2;
		d_viewpoint->y = size / 2;
		d_viewpoint->z = d_fractal_heightmap_get (d_terrain, d_viewpoint->x, d_viewpoint->y);
	}
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
	if (d_viewpoint) {
		d_gui_terminal_printf_left (5, 1, " N 37 23.516, W 122 02.625 (%d, %d), %dm ",
									d_viewpoint->x, d_viewpoint->y, d_viewpoint->z);
	}

	d_gui_terminal_printf_right (d_gui_terminal_size.width - 5, 1,
								 " Tuesday 15 march year 723 ");

	int middle = d_gui_terminal_size.height / 2;
	d_gui_terminal_printf_left (1, middle - 1, " ");
	d_gui_terminal_printf_left (1, middle, "W");
	d_gui_terminal_printf_left (1, middle + 1, " ");

	d_gui_terminal_printf_left (d_gui_terminal_size.width, middle - 1, " ");
	d_gui_terminal_printf_left (d_gui_terminal_size.width, middle, "E");
	d_gui_terminal_printf_left (d_gui_terminal_size.width, middle + 1, " ");

	d_gui_terminal_printf_center (d_gui_terminal_size.width / 2,
								  d_gui_terminal_size.height, " S ");

	d_gui_terminal_set_color (d_none, d_white);
}

void
d_gui_terminal_game_key (char key) {
	if (key == 'q') {
		d_quit = 1;
		return;
	}

	if (!d_terrain || !d_viewpoint) {
		return;
	}

	switch (key) {
	case 'w':
		if (d_viewpoint->y > 0) {
			d_viewpoint->y--;
		}
		break;
	case 'a':
		if (d_viewpoint->x > 0) {
			d_viewpoint->x--;
		}
		break;
	case 's':
		if (d_viewpoint->y < d_terrain->height - 1) {
			d_viewpoint->y++;
		}
		break;
	case 'd':
		if (d_viewpoint->x < d_terrain->width - 1) {
			d_viewpoint->x++;
		}
		break;

	case 'W':
		if (d_viewpoint->y > 9) {
			d_viewpoint->y-=10;
		}
		break;
	case 'A':
		if (d_viewpoint->x > 9) {
			d_viewpoint->x-=10;
		}
		break;
	case 'S':
		if (d_viewpoint->y < d_terrain->height - 11) {
			d_viewpoint->y+=10;
		}
		break;
	case 'D':
		if (d_viewpoint->x < d_terrain->width - 11) {
			d_viewpoint->x+=10;;
		}
		break;

	case 'z':
		break;
	case 'Z':
		break;
	case '<':
		d_viewpoint->z--;
		break;
	case '>':
		d_viewpoint->z++;
		break;
	case 'p':
		/* TODO pause the game. */
		break;
	default:
		printf ("Received key: %d\n", key);
	}
}
