#include "curses_io.h"
#include "curses_new_world.h"

static void d_cmd_world_to_main_menu_cb ();
static void d_cmd_world_save_cb ();
static void d_cmd_world_new_cb ();

static void d_curses_new_world_draw (struct d_ui_state *state);

struct d_ui_command d_cmd_world_to_main_menu = {
	"Main menu", d_cmd_world_to_main_menu_cb };
struct d_ui_command d_cmd_world_save = { "Save world", d_cmd_world_save_cb };
struct d_ui_command d_cmd_world_new = { "Generate new world", d_cmd_world_new_cb };

struct d_ui_state d_new_world_state = {
	"New world",
	0,
	0,
	d_curses_new_world_draw,
	{
		{ 'q', &d_cmd_world_to_main_menu },
		{ 's', &d_cmd_world_save },
		{ 'n', &d_cmd_world_new },
		{ 0, 0},
	}
};

static void
d_curses_new_world_draw (struct d_ui_state *state) {
	d_ui->clearscr ();

	d_ui->printf_left (1, 1, "TODO");
	/* TODO draw map and render a small menu with commands
	   and a part of the screen with basic information about the
	   map. */
}

static void
d_cmd_world_to_main_menu_cb () {
	/* TODO implement 'Are you sure you want to abandon this world.' */
	d_ui_do_transition (&d_transition_new_world_back);
}

static void
d_cmd_world_save_cb () {
	/* TODO implement save functionality. */
	d_ui_do_transition (&d_transition_new_world_back);
}

static void
d_cmd_world_new_cb () {
}
