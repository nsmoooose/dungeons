#include <stdio.h>

#include "client.h"
#include "error.h"
#include "fractal_heightmap.h"
#include "curses_game.h"
#include "curses_io.h"
#include "math.h"
#include "memory.h"
#include "ui.h"

static void d_curses_game_update (struct d_ui_handler *handler, double now, double delta);
static void d_curses_game_draw (struct d_ui_handler *handler);
static void d_curses_game_key (struct d_ui_handler *handler, int key);

struct d_ui_handler d_game_handler = {
	0,
	d_curses_game_update,
	d_curses_game_draw,
	d_curses_game_key
};

/* static struct d_client_context *d_context = 0; */
static struct d_heightmap *d_terrain = 0;
static struct d_ui_viewpoint *d_viewpoint=0;

/* Symbols to use:
   ~ =
   ▓ =
   ■ =
   · =
   ' =
   ▼ =
   ▲ =
*/

void
d_curses_draw_terrain () {
	if (!d_viewpoint || !d_terrain) {
		d_curses_set_color (d_green_white);
		d_curses_clear ();
		return;
	}

	for (int x=1;x<=d_curses_size.width-1;++x) {
		for (int y=1;y<=d_curses_size.height-1;++y) {
			int realx = d_viewpoint->x - d_curses_size.width / 2 + x;
			int realy = d_viewpoint->y - d_curses_size.height / 2 + y;
			if (realx < 0 || realy < 0 || realx > d_terrain->width || realy > d_terrain->height) {
				d_curses_set_color (d_black_white);
				d_curses_printf_left (x, y, "X");
				continue;
			}

			/* Our point can be in four different places.
			   - sea.
			   - air
			   - below ground level.
			   - ground level.
			*/

			float f = d_fractal_heightmap_get (d_terrain, realx, realy);
			if (d_viewpoint->z - 15.0 > f && d_viewpoint->z <= 0.0) {
				/* above ground but below sea level. */
				d_curses_set_color (d_black_blue);
				d_curses_printf_left (x, y, "~");
			}
			else if (f <= d_viewpoint->z + 15.0 && f >= d_viewpoint->z - 15.0) {
				if (f <= 0.0) {
					d_curses_set_color (d_black_blue);
				}
				else {
					/* ground level. */
					d_curses_set_color (d_black_green);
				}
				d_curses_printf_left (x, y, ".");
			}
			else if (d_viewpoint->z + 15.0 > f && d_viewpoint->z > 0.0) {
				/* above ground and above sea level. */
				d_curses_set_color (d_black_cyan);
				d_curses_printf_left (x, y, "'");
			}
			else if (d_viewpoint->z - 15.0 < f) {
				/* below ground level. */
				d_curses_set_color (d_black_white);
				d_curses_printf_left (x, y, " ");
			}
			else {
				d_bug ("Not supposed to happen. Viewpoint z: %d, Ground level: %f", d_viewpoint->z, f);
			}
		}
	}
}

static void
d_curses_game_update (struct d_ui_handler *handler, double now, double delta) {
	if (!d_terrain) {
		int size = 2048;
		d_terrain = d_fractal_heightmap_new (size);
		d_fractal_heightmap_generate (d_terrain, 123, 10000.f, 0.9f);
		d_viewpoint = d_calloc (1, sizeof (struct d_ui_viewpoint));
		d_viewpoint->x = size / 2;
		d_viewpoint->y = size / 2;
		d_viewpoint->z = fmax (d_fractal_heightmap_get (d_terrain, d_viewpoint->x, d_viewpoint->y), 0.0f);
	}
}

static void
d_curses_game_draw (struct d_ui_handler *handler) {
	d_curses_draw_terrain ();

	d_curses_set_color (d_white_black);
	d_curses_box (0, 0, d_curses_size.width-1,
						d_curses_size.height-1, ' ');
	d_curses_set_color (d_black_green);
	d_curses_printf_center (d_curses_size.width / 2, 0,
								  "|======- D U N G E O N S -======|");

	d_curses_set_color (d_black_white);
	if (d_viewpoint) {
		d_curses_printf_left (5, 0, " N 37 23.516, W 122 02.625 (%d, %d), %dm ",
									d_viewpoint->x, d_viewpoint->y, d_viewpoint->z);
	}

	d_curses_printf_right (d_curses_size.width - 5, 0,
								 " Tuesday 15 march year 723 ");

	int middle = d_curses_size.height / 2;
	d_curses_printf_left (0, middle - 1, " ");
	d_curses_printf_left (0, middle, "W");
	d_curses_printf_left (0, middle + 1, " ");

	d_curses_printf_left (d_curses_size.width-1, middle - 1, " ");
	d_curses_printf_left (d_curses_size.width-1, middle, "E");
	d_curses_printf_left (d_curses_size.width-1, middle + 1, " ");

	d_curses_printf_center (d_curses_size.width / 2,
								  d_curses_size.height-1, " S ");

	d_curses_set_color (d_black_white);
}

static void
d_curses_game_key (struct d_ui_handler *handler, int key) {
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
		d_viewpoint->z-=10;
		break;
	case '>':
		d_viewpoint->z+=10;
		break;
	case 'p':
		/* TODO pause the game. */
		break;
	default:
		break;
	}
}
