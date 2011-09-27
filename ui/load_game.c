#include <unistd.h>

#include "game.h"
#include "load_game.h"

static void d_cmd_load_game_abort_cb ();
static void d_cmd_load_game_directory_cb ();
static void d_cmd_load_game_next_cb ();
static void d_cmd_load_game_prev_cb ();

static void d_load_game_update (struct d_ui_state *handler, double now, double delta);
static void d_load_game_draw (struct d_ui_state *handler);

struct d_ui_command d_cmd_load_game_abort = { "Abort", d_cmd_load_game_abort_cb };
struct d_ui_command d_cmd_load_game_directory = { "Load", d_cmd_load_game_directory_cb };
struct d_ui_command d_cmd_load_game_next = { "Next", d_cmd_load_game_next_cb };
struct d_ui_command d_cmd_load_game_prev = { "Previous", d_cmd_load_game_prev_cb };

static struct d_list *d_loadable_games = 0;
static struct d_list_node *d_selected_game = 0;

struct d_ui_state d_load_game_state = {
	"Load game",
	0,
	d_load_game_update,
	d_load_game_draw,
	{
		{ 'a', &d_cmd_load_game_abort },
		{ 'l', &d_cmd_load_game_directory },
		{ 'n', &d_cmd_load_game_next },
		{ 'p', &d_cmd_load_game_prev },
		{ 0, 0 }
	}
};

static void
d_load_game_update (struct d_ui_state *handler, double now, double delta) {
	usleep (100000);
}

static void
d_load_game_draw (struct d_ui_state *handler) {
	if (!d_loadable_games) {
		d_loadable_games = d_game_info_list ();
	}
	d_ui->clearscr ();
	d_ui->title_large_draw ();
	struct d_list_node *node = d_loadable_games->first;
	for (int i=15;node;node=node->next,++i) {
		if (!d_selected_game) {
			d_selected_game = node;
		}
		struct d_game_info *info = node->data;
		if (node == d_selected_game) {
			d_ui->set_color (d_white_black);
		}
		else {
			d_ui->set_color (d_black_white);
		}
		d_ui->printf_center (d_ui->size.width/2, i, "%-40s", info->description);
	}
}

static void
d_cmd_load_game_abort_cb () {
	d_ui_do_transition (&d_transition_load_game_abort);
	d_list_destroy (d_loadable_games);
}

static void
d_cmd_load_game_directory_cb () {
	d_ui_do_transition (&d_transition_load_game_loading);
	d_list_destroy (d_loadable_games);
}

static void
d_cmd_load_game_next_cb () {
	if (d_selected_game && d_selected_game->next) {
		d_selected_game = d_selected_game->next;
	}
}

static void
d_cmd_load_game_prev_cb () {
	if (d_selected_game && d_selected_game->prev) {
		d_selected_game = d_selected_game->prev;
	}
}

