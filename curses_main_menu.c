#include <unistd.h>

#include "clock.h"
#include "error.h"
#include "curses_game.h"
#include "curses_io.h"
#include "curses_main_menu.h"

const char *d_curses_game_title[] = {
	"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
	"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
	"D     D U    U N N  N G      E     O    O  N N  N S     ",
	"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
	"D     D U    U N  N N G    G E     O    O  N  N N      S",
	"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
	"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
};

static void d_cmd_new_game_cb ();
static void d_cmd_load_game_cb ();
static void d_cmd_explore_cb ();
static void d_cmd_quit_cb ();

static void d_curses_main_menu_update (struct d_ui_handler *handler, double now, double delta);
static void d_curses_main_menu_draw (struct d_ui_handler *handler);

struct d_ui_command d_cmd_new_game = { "New game", d_cmd_new_game_cb };
struct d_ui_command d_cmd_load_game = { "Load game", d_cmd_load_game_cb };
struct d_ui_command d_cmd_explore = { "Explore", d_cmd_explore_cb };
struct d_ui_command d_cmd_quit = { "Quit", d_cmd_quit_cb };

struct d_ui_key_binding d_main_menu[] = {
	{ 'n', &d_cmd_new_game },
 	{ 'c', &d_cmd_load_game },
	{ 'e', &d_cmd_explore },
	{ 'q', &d_cmd_quit },
	{ 0, 0 }
};

struct d_ui_handler d_main_menu_handler = {
	0,
	d_curses_main_menu_update,
	d_curses_main_menu_draw,
	0
};

static void
d_cmd_new_game_cb () {
	d_ui_stack_push (&d_game_handler);
}

static void
d_cmd_load_game_cb () {
}

static void
d_cmd_explore_cb () {
}

static void
d_cmd_quit_cb () {
	d_quit = 1;
}

static void
d_curses_main_menu_update (struct d_ui_handler *handler, double now, double delta) {
	usleep (100000);
}

static void
d_curses_main_menu_draw (struct d_ui_handler *handler) {
	d_curses_set_color (d_black_green);
	d_curses_clear ();

	int center = d_curses_size.width / 2;

	for (int i=0;i<7;++i) {
		d_curses_printf_center (center, 3 + i, d_curses_game_title[i]);
	}
}
