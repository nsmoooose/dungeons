#include "load_game.h"

static void d_cmd_load_game_abort_cb ();
static void d_cmd_load_game_directory_cb ();
static void d_cmd_load_game_next_cb ();
static void d_cmd_load_game_prev_cb ();

struct d_ui_command d_cmd_load_game_abort = { "Abort", d_cmd_load_game_abort_cb };
struct d_ui_command d_cmd_load_game_directory = { "Load", d_cmd_load_game_directory_cb };
struct d_ui_command d_cmd_load_game_next = { "Next", d_cmd_load_game_next_cb };
struct d_ui_command d_cmd_load_game_prev = { "Previous", d_cmd_load_game_prev_cb };

struct d_ui_state d_load_game_state = {
	"Load game",
	0,
	0,
	0,
	{
		{ 'a', &d_cmd_load_game_abort },
		{ 'l', &d_cmd_load_game_directory },
		{ 'n', &d_cmd_load_game_next },
		{ 'p', &d_cmd_load_game_prev },
		{ 0, 0 }
	}
};

static void
d_cmd_load_game_abort_cb () {
	d_ui_do_transition (&d_transition_load_game_abort);
}

static void
d_cmd_load_game_directory_cb () {
}

static void
d_cmd_load_game_next_cb () {
}

static void
d_cmd_load_game_prev_cb () {
}

