#include "curses_new_world.h"

static void d_cmd_world_to_main_menu_cb ();
static void d_cmd_world_save_cb ();
static void d_cmd_world_new_cb ();

static void d_curses_new_world_draw (struct d_ui_state *state);

struct d_ui_command d_cmd_world_to_main_menu = { "Main menu", d_cmd_world_to_main_menu_cb };
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
}

static void
d_cmd_world_to_main_menu_cb () {
}

static void
d_cmd_world_save_cb () {
}

static void
d_cmd_world_new_cb () {
}
