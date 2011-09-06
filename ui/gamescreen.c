#include <stdio.h>

#include "client.h"
#include "error.h"
#include "game.h"
#include "gamescreen.h"
#include "fractal_heightmap.h"
#include "curses_io.h"
#include "curses_map.h"
#include "math.h"
#include "memory.h"
#include "ui.h"

static void d_gamescreen_update (struct d_ui_state *handler, double now, double delta);
static void d_gamescreen_draw (struct d_ui_state *handler);

static void d_cmd_map_pan_right_cb ();
static void d_cmd_map_pan_left_cb ();
static void d_cmd_map_pan_up_cb ();
static void d_cmd_map_pan_down_cb ();

static void d_cmd_map_pan_right_fast_cb ();
static void d_cmd_map_pan_left_fast_cb ();
static void d_cmd_map_pan_up_fast_cb ();
static void d_cmd_map_pan_down_fast_cb ();

static void d_cmd_map_down_cb ();
static void d_cmd_map_up_cb ();

static void d_cmd_map_zoom_in_cb ();
static void d_cmd_map_zoom_out_cb ();

static void d_cmd_esc_menu_cb ();

struct d_ui_command d_cmd_map_pan_right = { "Scroll map right", d_cmd_map_pan_right_cb };
struct d_ui_command d_cmd_map_pan_left = { "Scroll map left", d_cmd_map_pan_left_cb };
struct d_ui_command d_cmd_map_pan_up = { "Scroll map up", d_cmd_map_pan_up_cb };
struct d_ui_command d_cmd_map_pan_down = { "Scroll map down", d_cmd_map_pan_down_cb };

struct d_ui_command d_cmd_map_pan_right_fast = { "Scroll map right fast", d_cmd_map_pan_right_fast_cb };
struct d_ui_command d_cmd_map_pan_left_fast = { "Scroll map left fast", d_cmd_map_pan_left_fast_cb };
struct d_ui_command d_cmd_map_pan_up_fast = { "Scroll map up fast", d_cmd_map_pan_up_fast_cb };
struct d_ui_command d_cmd_map_pan_down_fast = { "Scroll map down fast", d_cmd_map_pan_down_fast_cb };

struct d_ui_command d_cmd_map_down = { "Down", d_cmd_map_down_cb };
struct d_ui_command d_cmd_map_up = { "Up", d_cmd_map_up_cb };

struct d_ui_command d_cmd_map_zoom_in = { "Zoom in", d_cmd_map_zoom_in_cb };
struct d_ui_command d_cmd_map_zoom_out = { "Zoom out", d_cmd_map_zoom_out_cb };

struct d_ui_command d_cmd_esc_menu = { "In game menu", d_cmd_esc_menu_cb };

struct d_ui_state d_gamescreen_state = {
	"Game",
	0,
	d_gamescreen_update,
	d_gamescreen_draw,
	{
		{ 'a', &d_cmd_map_pan_right },
		{ 'd', &d_cmd_map_pan_left },
		{ 'w', &d_cmd_map_pan_down },
		{ 's', &d_cmd_map_pan_up },

		{ 'A', &d_cmd_map_pan_right_fast },
		{ 'D', &d_cmd_map_pan_left_fast },
		{ 'W', &d_cmd_map_pan_down_fast },
		{ 'S', &d_cmd_map_pan_up_fast },

		{ '<', &d_cmd_map_down },
		{ '>', &d_cmd_map_up },

		{ '+', &d_cmd_map_zoom_in },
		{ '-', &d_cmd_map_zoom_out },

		{ 'q', &d_cmd_esc_menu },

		{ 0, 0 }
	}
};

/* static struct d_client_context *d_context = 0; */
struct d_game_context *d_context = 0;
static struct d_heightmap *d_terrain = 0;
static struct d_ui_viewpoint *d_viewpoint=0;
static int d_zoom_level = 1;

static void
d_gamescreen_draw_terrain () {
	if (!d_viewpoint || !d_terrain) {
		d_ui->set_color (d_green_white);
		d_ui->clearscr ();
		return;
	}

	struct d_ui_area area;
	area.pos.x = area.pos.y = 1;
	area.size.width = d_curses_size.width - 1;
	area.size.height = d_curses_size.height - 1;
	d_curses_map_draw (&area, d_viewpoint, d_zoom_level, d_terrain);
}

static void
d_gamescreen_update (struct d_ui_state *handler, double now, double delta) {
	if (!d_context) {
		d_context = d_game_context_new ();
	}
	if (!d_terrain) {
		int size = 2048;
		d_terrain = d_fractal_heightmap_new (size);
		d_fractal_heightmap_generate (d_terrain, 123, 10000.f, 0.9f);
		d_viewpoint = d_calloc (1, sizeof (struct d_ui_viewpoint));
		d_viewpoint->x = size / 2;
		d_viewpoint->y = size / 2;
		d_viewpoint->z = fmax (d_fractal_heightmap_get (d_terrain, d_viewpoint->x, d_viewpoint->y), 0.0f);
	}
	d_game_run (d_context, now, delta);
}

static void
d_gamescreen_draw (struct d_ui_state *handler) {
	d_gamescreen_draw_terrain ();

	d_ui->set_color (d_white_black);
	d_ui->box (0, 0, d_curses_size.width-1,
			   d_curses_size.height-1, ' ');
	d_ui->set_color (d_black_green);
	d_ui->printf_center (d_curses_size.width / 2, 0,
								  "|======- D U N G E O N S -======|");

	d_ui->set_color (d_black_white);
	if (d_viewpoint) {
		d_ui->printf_left (5, 0, " N 37 23.516, W 122 02.625 (%d, %d), %dm ",
						   d_viewpoint->x, d_viewpoint->y, d_viewpoint->z);
	}

	char date[30];
	d_game_format_date (date, 30, d_context->datetime);
	d_ui->printf_right (d_curses_size.width - 5, 0,
						   date);

	int middle = d_curses_size.height / 2;
	d_ui->printf_left (0, middle - 1, " ");
	d_ui->printf_left (0, middle, "W");
	d_ui->printf_left (0, middle + 1, " ");

	d_ui->printf_left (d_curses_size.width-1, middle - 1, " ");
	d_ui->printf_left (d_curses_size.width-1, middle, "E");
	d_ui->printf_left (d_curses_size.width-1, middle + 1, " ");

	d_ui->printf_center (d_curses_size.width / 2,
						 d_curses_size.height-1, " S ");

	d_ui->set_color (d_black_white);
}


static void
d_cmd_map_pan_right_cb () {
	d_viewpoint->x = fmax (d_viewpoint->x - 1, 0);
}

static void
d_cmd_map_pan_left_cb () {
	d_viewpoint->x = fmin (d_viewpoint->x + 1, d_terrain->width);
}

static void
d_cmd_map_pan_up_cb () {
	d_viewpoint->y = fmin (d_viewpoint->y + 1, d_terrain->height);
}

static void
d_cmd_map_pan_down_cb () {
	d_viewpoint->y = fmax (d_viewpoint->y - 1, 0);
}

static void
d_cmd_map_pan_right_fast_cb () {
	d_viewpoint->x = fmax (d_viewpoint->x - 10 * d_zoom_level, 0);
}

static void
d_cmd_map_pan_left_fast_cb () {
	d_viewpoint->x = fmin (d_viewpoint->x + 10 * d_zoom_level, d_terrain->width);
}

static void
d_cmd_map_pan_up_fast_cb () {
	d_viewpoint->y = fmin (d_viewpoint->y + 10 * d_zoom_level, d_terrain->height);
}

static void
d_cmd_map_pan_down_fast_cb () {
	d_viewpoint->y = fmax (d_viewpoint->y - 10 * d_zoom_level, 0);
}

static void
d_cmd_map_zoom_in_cb () {
	d_zoom_level = fmax (1, d_zoom_level - 1);
}

static void
d_cmd_map_zoom_out_cb () {
	d_zoom_level = fmin (15, d_zoom_level + 1);
}

static void
d_cmd_map_down_cb () {
	d_viewpoint->z-=10;
}

static void
d_cmd_map_up_cb () {
	d_viewpoint->z+=10;
}

static void
d_cmd_esc_menu_cb () {
	d_ui_do_transition (&d_transition_esc_menu);
}
