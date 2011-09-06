#include <unistd.h>

#include "clock.h"
#include "curses_game.h"
#include "curses_io.h"
#include "error.h"
#include "main_menu.h"
#include "new_world.h"
#include "quit.h"

static void d_cmd_new_game_cb ();
static void d_cmd_new_world_cb ();
static void d_cmd_load_game_cb ();
static void d_cmd_explore_cb ();
static void d_cmd_quit_cb ();

static void d_main_menu_update (
	struct d_ui_state *state, double now, double delta);
static void d_main_menu_draw (struct d_ui_state *state);

struct d_ui_command d_cmd_new_game = { "New game", d_cmd_new_game_cb };
struct d_ui_command d_cmd_new_world = { "New world", d_cmd_new_world_cb };
struct d_ui_command d_cmd_load_game = { "Load game", d_cmd_load_game_cb };
struct d_ui_command d_cmd_explore = { "Explore", d_cmd_explore_cb };
struct d_ui_command d_cmd_quit = { "Quit", d_cmd_quit_cb };

struct d_ui_state d_main_menu_state = {
	"Main menu",
	0,
	d_main_menu_update,
	d_main_menu_draw,
	{
		{ 'w', &d_cmd_new_world },
		{ 'n', &d_cmd_new_game },
		{ 'c', &d_cmd_load_game },
		{ 'e', &d_cmd_explore },
		{ 'q', &d_cmd_quit },
		{ 0, 0 }
	}
};

static void
d_cmd_new_game_cb () {
	d_ui_do_transition (&d_transition_new_game);
}

static void
d_cmd_new_world_cb () {
	d_ui_do_transition (&d_transition_new_world);
}

static void
d_cmd_load_game_cb () {
}

static void
d_cmd_explore_cb () {
}

static void
d_cmd_quit_cb () {
	d_ui_do_transition (&d_transition_quit_question);
}

static void
d_main_menu_update (struct d_ui_state *state, double now, double delta) {
	usleep (100000);
}

static void
d_main_menu_draw (struct d_ui_state *state) {
	d_ui->clearscr ();
	d_ui->title_large_draw ();
	struct d_ui_area area = { { d_curses_size.width / 2 - 10, 13}, { 20, 20 } };
	d_ui->menu_draw (&area, state->key_bindings);
}
