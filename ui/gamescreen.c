#include <math.h>
#include <stdio.h>

#include "client.h"
#include "error.h"
#include "game.h"
#include "gamescreen.h"
#include "heightmap.h"
#include "map.h"
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

static void d_cmd_look_around_cb ();

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

struct d_ui_command d_cmd_look_around = { "Look around", d_cmd_look_around_cb };

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

		{ 'l', &d_cmd_look_around },

		{ 'q', &d_cmd_esc_menu },

		{ 0, 0 }
	}
};

/* static struct d_client_context *d_context = 0; */
struct d_game_context *d_context = 0;

static void
d_gamescreen_draw_terrain () {
	struct d_ui_area area;
	area.pos.x = area.pos.y = 1;
	area.size.width = d_ui->size.width - 1;
	area.size.height = d_ui->size.height - 1;
	d_map_draw (&area, d_context->vp, d_context->zoom_level, d_context->hm);
}

static void
d_gamescreen_update (struct d_ui_state *handler, double now, double delta) {
	if (!d_context) {
		d_bug ("No context assign. Cannot run a game.");
	}
	d_game_run (d_context, now, delta);
}

static void
d_gamescreen_draw (struct d_ui_state *handler) {
	d_gamescreen_draw_terrain ();

	d_ui->set_color (d_white_black);
	d_ui->box (0, 0, d_ui->size.width-1,
			   d_ui->size.height-1, ' ');
	d_ui->set_color (d_black_green);
	d_ui->printf_center (d_ui->size.width / 2, 0,
								  "|======- D U N G E O N S -======|");

	d_ui->set_color (d_black_white);
	if (d_context->vp) {
		d_ui->printf_left (5, 0, " N 37 23.516, W 122 02.625 (%d, %d), %dm ",
						   d_context->vp->x, d_context->vp->y, d_context->vp->z);
	}

	char date[30];
	d_game_format_date (date, 30, d_context->datetime);
	d_ui->printf_right (d_ui->size.width - 5, 0,
						date);

	int middle = d_ui->size.height / 2;
	d_ui->printf_left (0, middle - 1, " ");
	d_ui->printf_left (0, middle, "W");
	d_ui->printf_left (0, middle + 1, " ");

	d_ui->printf_left (d_ui->size.width-1, middle - 1, " ");
	d_ui->printf_left (d_ui->size.width-1, middle, "E");
	d_ui->printf_left (d_ui->size.width-1, middle + 1, " ");

	d_ui->printf_center (d_ui->size.width / 2,
						 d_ui->size.height-1, " S ");

	d_ui->set_color (d_black_white);
}


static void
d_cmd_map_pan_right_cb () {
	d_context->vp->x = fmax (d_context->vp->x - 1, 0);
}

static void
d_cmd_map_pan_left_cb () {
	d_context->vp->x = fmin (d_context->vp->x + 1, d_context->hm->width);
}

static void
d_cmd_map_pan_up_cb () {
	d_context->vp->y = fmin (d_context->vp->y + 1, d_context->hm->height);
}

static void
d_cmd_map_pan_down_cb () {
	d_context->vp->y = fmax (d_context->vp->y - 1, 0);
}

static void
d_cmd_map_pan_right_fast_cb () {
	d_context->vp->x = fmax (d_context->vp->x - 10 * d_context->zoom_level, 0);
}

static void
d_cmd_map_pan_left_fast_cb () {
	d_context->vp->x = fmin (d_context->vp->x + 10 * d_context->zoom_level, d_context->hm->width);
}

static void
d_cmd_map_pan_up_fast_cb () {
	d_context->vp->y = fmin (d_context->vp->y + 10 * d_context->zoom_level, d_context->hm->height);
}

static void
d_cmd_map_pan_down_fast_cb () {
	d_context->vp->y = fmax (d_context->vp->y - 10 * d_context->zoom_level, 0);
}

static void
d_cmd_map_zoom_in_cb () {
	d_context->zoom_level = fmax (1, d_context->zoom_level - 1);
}

static void
d_cmd_map_zoom_out_cb () {
	d_context->zoom_level = fmin (15, d_context->zoom_level + 1);
}

static void
d_cmd_map_down_cb () {
	d_context->vp->z-=10;
}

static void
d_cmd_map_up_cb () {
	d_context->vp->z+=10;
}

static void
d_cmd_esc_menu_cb () {
	d_ui_do_transition (&d_transition_esc_menu);
}

static void
d_cmd_look_around_cb () {
	d_ui_do_transition (&d_transition_look_around);
}

/* *************************************************************************
   Look around state
   ************************************************************************* */

static void d_cmd_marker_down_cb ();
static void d_cmd_marker_left_cb ();
static void d_cmd_marker_right_cb ();
static void d_cmd_marker_up_cb ();
static void d_cmd_look_around_abandon_cb ();

struct d_ui_command d_cmd_marker_down = { "Move marker down", d_cmd_marker_down_cb };
struct d_ui_command d_cmd_marker_left = { "Move marker left", d_cmd_marker_left_cb };
struct d_ui_command d_cmd_marker_right = { "Move marker right", d_cmd_marker_right_cb };
struct d_ui_command d_cmd_marker_up = { "Move marker up", d_cmd_marker_up_cb };
struct d_ui_command d_cmd_look_around_abandon = { "Return", d_cmd_look_around_abandon_cb };

struct d_ui_state d_look_around_state = {
	"Look around",
	0,
	d_gamescreen_update,
	d_gamescreen_draw,
	{
		{ 'a', &d_cmd_marker_right },
		{ 'd', &d_cmd_marker_left },
		{ 's', &d_cmd_marker_up },
		{ 'w', &d_cmd_marker_down },

		{ 'q', &d_cmd_look_around_abandon },

		{ 0, 0 }
	}
};

static void d_cmd_marker_down_cb () {
}

static void d_cmd_marker_left_cb () {
}

static void d_cmd_marker_right_cb () {
}

static void d_cmd_marker_up_cb () {
}

static void d_cmd_look_around_abandon_cb () {
	d_ui_do_transition (&d_transition_look_around_return);
}
