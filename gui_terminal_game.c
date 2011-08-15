#include <stdio.h>

#include "client.h"
#include "error.h"
#include "fractal_heightmap.h"
#include "gui_terminal_game.h"
#include "gui_terminal_io.h"
#include "math.h"
#include "memory.h"
#include "ui.h"

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
d_gui_terminal_draw_terrain () {
	if (!d_viewpoint || !d_terrain) {
		d_gui_terminal_set_color (d_green_white);
		d_gui_terminal_clear ();
		return;
	}

	for (int x=1;x<=d_gui_terminal_size.width-1;++x) {
		for (int y=1;y<=d_gui_terminal_size.height-1;++y) {
			int realx = d_viewpoint->x - d_gui_terminal_size.width / 2 + x;
			int realy = d_viewpoint->y - d_gui_terminal_size.height / 2 + y;
			if (realx < 0 || realy < 0 || realx > d_terrain->width || realy > d_terrain->height) {
				d_gui_terminal_set_color (d_black_white);
				d_gui_terminal_printf_left (x, y, "X");
				continue;
			}

			/* Our point can be in four different places.
			   - sea.
			   - air
			   - below ground level.
			   - ground level.
			*/

			float f = d_fractal_heightmap_get (d_terrain, realx, realy);
			if (d_viewpoint->z - 15.0 > f && f < 0.0) {
				/* above ground but below sea level. */
				d_gui_terminal_set_color (d_black_blue);
				d_gui_terminal_printf_left (x, y, "~");
			}
			else if (d_viewpoint->z + 15.0 > f && f > 0.0) {
				/* above ground and above sea level. */
				d_gui_terminal_set_color (d_cyan_white);
				d_gui_terminal_printf_left (x, y, " ");
			}
			else if (d_viewpoint->z - 15.0 < f) {
				/* below ground level. */
				d_gui_terminal_set_color (d_black_white);
				d_gui_terminal_printf_left (x, y, " ");
			}
			else if (d_viewpoint->z + 15.0 >= f && d_viewpoint->z - 15.0 <= f) {
				/* ground level. */
				d_gui_terminal_set_color (d_black_green);
				d_gui_terminal_printf_left (x, y, ".");
			}
			else {
				d_bug ("Not supposed to happen. Viewpoint z: %d, Ground level: %f", d_viewpoint->z, f);
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
		d_viewpoint = d_calloc (1, sizeof (struct d_ui_viewpoint));
		d_viewpoint->x = size / 2;
		d_viewpoint->y = size / 2;
		d_viewpoint->z = fmax (d_fractal_heightmap_get (d_terrain, d_viewpoint->x, d_viewpoint->y), 0.0f);
	}
}

void
d_gui_terminal_game_draw () {
	d_gui_terminal_draw_terrain ();

	d_gui_terminal_set_color (d_white_black);
	d_gui_terminal_box (0, 0, d_gui_terminal_size.width-1,
						d_gui_terminal_size.height-1, ' ');
	d_gui_terminal_set_color (d_black_green);
	d_gui_terminal_printf_center (d_gui_terminal_size.width / 2, 0,
								  "|======- D U N G E O N S -======|");

	d_gui_terminal_set_color (d_black_white);
	if (d_viewpoint) {
		d_gui_terminal_printf_left (5, 0, " N 37 23.516, W 122 02.625 (%d, %d), %dm ",
									d_viewpoint->x, d_viewpoint->y, d_viewpoint->z);
	}

	d_gui_terminal_printf_right (d_gui_terminal_size.width - 5, 0,
								 " Tuesday 15 march year 723 ");

	int middle = d_gui_terminal_size.height / 2;
	d_gui_terminal_printf_left (0, middle - 1, " ");
	d_gui_terminal_printf_left (0, middle, "W");
	d_gui_terminal_printf_left (0, middle + 1, " ");

	d_gui_terminal_printf_left (d_gui_terminal_size.width-1, middle - 1, " ");
	d_gui_terminal_printf_left (d_gui_terminal_size.width-1, middle, "E");
	d_gui_terminal_printf_left (d_gui_terminal_size.width-1, middle + 1, " ");

	d_gui_terminal_printf_center (d_gui_terminal_size.width / 2,
								  d_gui_terminal_size.height-1, " S ");

	d_gui_terminal_set_color (d_black_white);
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
