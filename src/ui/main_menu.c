#include <math.h>
#include <unistd.h>

#include "../clock.h"
#include "../error.h"
#include "../fractal_heightmap.h"
#include "../memory.h"
#include "../object.h"
#include "../str.h"
#include "../world_gen.h"
#include "gamescreen.h"
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
	0, /* enter */
	0, /* exit */
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
	/* Set a context */
	d_context = d_game_context_new ();

	struct d_world_gen_params params = { 128 };
	d_context->world = d_world_generate (&params);

	d_context->directory = d_strdup ("/home/henrikn/.dungeons/saves/test");
	d_context->vp->x = params.size / 2;
	d_context->vp->y = params.size / 2;
	d_context->vp->z = fmax (d_heightmap_get (d_context->world->hm, d_context->vp->x, d_context->vp->y), 0.0f);

	d_context->zoom_level = 1;

	struct d_ob_type *tree_type = d_ob_get_type (&d_ob_registry, "picea");
	short height = d_heightmap_get (d_context->world->hm, 10, 10);
	struct d_ob_instance *tree = tree_type->create (tree_type, 10, 10, height);
	d_list_append (d_context->objects, tree);

	d_ui_do_transition (&d_transition_new_game);
}

static void
d_cmd_new_world_cb () {
	d_ui_do_transition (&d_transition_new_world);
}

static void
d_cmd_load_game_cb () {
	d_ui_do_transition (&d_transition_load_game);
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
	struct d_ui_area area = { { d_ui->size.width / 2 - 10, 13}, { 20, 20 } };
	d_ui->menu_draw (&area, state->key_bindings);
}
