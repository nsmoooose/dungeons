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

struct d_command d_cmd_new_game = { "New game", d_cmd_new_game_cb };
struct d_command d_cmd_load_game = { "Load game", d_cmd_load_game_cb };
struct d_command d_cmd_explore = { "Explore", d_cmd_explore_cb };
struct d_command d_cmd_quit = { "Quit", d_cmd_quit_cb };

struct d_key_binding d_main_menu[] = {
	{ 'a', &d_cmd_new_game },
 	{ 'c', &d_cmd_load_game },
	{ 'e', &d_cmd_explore },
	{ 'q', &d_cmd_quit }
};

static void
d_cmd_new_game_cb () {
	d_curses_update = d_curses_game_update;
	d_curses_draw = d_curses_game_draw;
	d_curses_key = d_curses_game_key;
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

void
d_curses_main_menu_update (double now, double delta) {
	usleep (100000);
}

void
d_curses_main_menu_draw () {
	d_curses_set_color (d_black_green);
	d_curses_clear ();

	int center = d_curses_size.width / 2;

	for (int i=0;i<7;++i) {
		d_curses_printf_center (center, 3 + i, d_curses_game_title[i]);
	}

	d_curses_set_color (d_black_white);
	d_curses_printf_center (center, 13, "n) New game");
	d_curses_printf_center (center, 14, "c) Continue");
	d_curses_printf_center (center, 15, "e) Explore ");
	d_curses_printf_center (center, 16, "q) Quit    ");
}

void
d_curses_main_menu_key (char key) {
	switch (key) {
	case 'n':
	case 'N':
		d_curses_update = d_curses_game_update;
		d_curses_draw = d_curses_game_draw;
		d_curses_key = d_curses_game_key;
		break;

	case 'c':
	case 'C':
		/* TODO Load a previously saved game. */
		break;

	case 'e':
	case 'E':
		/* TODO just lookaround without playing. */
		break;

	case 'q':
	case 'Q':
		d_quit = 1;
		break;
	}
}
