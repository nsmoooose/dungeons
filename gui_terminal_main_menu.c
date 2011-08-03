#include <unistd.h>

#include "clock.h"
#include "error.h"
#include "gui_terminal_game.h"
#include "gui_terminal_io.h"
#include "gui_terminal_main_menu.h"

const char *d_gui_terminal_game_title[] = {
	"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
	"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
	"D     D U    U N N  N G      E     O    O  N N  N S     ",
	"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
	"D     D U    U N  N N G    G E     O    O  N  N N      S",
	"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
	"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
};

void
d_gui_terminal_main_menu_update (double now, double delta) {
	usleep (100000);
}

void
d_gui_terminal_main_menu_draw () {
	d_gui_terminal_set_color (d_black, d_green);
	d_gui_terminal_clear ();

	int center = d_gui_terminal_size.width / 2;

	for (int i=0;i<7;++i) {
		d_gui_terminal_printf_center (center, 3 + i, d_gui_terminal_game_title[i]);
	}

	d_gui_terminal_set_color (d_black, d_light_gray);
	d_gui_terminal_printf_center (center, 13, "n) New game");
	d_gui_terminal_printf_center (center, 14, "c) Continue");
	d_gui_terminal_printf_center (center, 15, "e) Explore ");
	d_gui_terminal_printf_center (center, 16, "q) Quit    ");
}

void
d_gui_terminal_main_menu_key (char key) {
	switch (key) {
	case 'n':
	case 'N':
		d_gui_terminal_update = d_gui_terminal_game_update;
		d_gui_terminal_draw = d_gui_terminal_game_draw;
		d_gui_terminal_key = d_gui_terminal_game_key;
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
