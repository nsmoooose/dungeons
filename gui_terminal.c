#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "error.h"
#include "gui_terminal.h"

const char *d_gui_terminal_game_title[] = {
	"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
	"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
	"D     D U    U N N  N G      E     O    O  N N  N S     ",
	"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
	"D     D U    U N  N N G    G E     O    O  N  N N      S",
	"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
	"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
};

struct {
	int width;
	int height;
} d_gui_terminal_size;

void
d_gui_terminal_update_size () {
	struct winsize ws;
	if (ioctl(0, TIOCGWINSZ, &ws) == -1) {
		d_bug ("Failed to get terminal size.");
	}
	if (d_gui_terminal_size.width != ws.ws_col ||
		d_gui_terminal_size.height != ws.ws_row) {
		d_gui_terminal_clear ();
	}

	d_gui_terminal_size.width = ws.ws_col;
	d_gui_terminal_size.height = ws.ws_row;
}

void
d_gui_terminal_clear () {
	printf ("\033[2J");
}

void
d_gui_terminal_setpos (int x, int y) {
	printf ("\033[%d;%dH", y, x);
}

void
d_gui_terminal_printf_center (int x, int y, const char *format) {
	int width = strlen (format);
	d_gui_terminal_setpos (x - (width / 2), y);
	printf (format);
}

void
d_gui_terminal_hide_cursor () {
	printf ("\033[?25l");
}

void
d_gui_terminal_show_cursor () {
	printf ("\033[?25h");
}

void
d_gui_terminal_set_style (enum d_gui_terminal_style style) {
	switch (style) {
	case d_normal:
		break;
	case d_bold:
		break;
	case d_italic:
		break;
	default:
		d_bug ("Invalid terminal style");
	}
}

void
d_gui_terminal_set_text_color (enum d_gui_terminal_color color) {
	switch (color) {
	case d_black:
		printf ("\033[0;30m");
		break;
	case d_dark_gray:
		printf ("\033[1;30m");
		break;
	case d_blue:
		printf ("\033[0;34m");
		break;
	case d_light_blue:
		printf ("\033[1;34m");
		break;
	case d_green:
		printf ("\033[0;32m");
		break;
	case d_light_green:
		printf ("\033[1;32m");
		break;
	case d_cyan:
		printf ("\033[0;36m");
		break;
	case d_light_cyan:
		printf ("\033[1;36m");
		break;
	case d_red:
		printf ("\033[0;31m");
		break;
	case d_light_red:
		printf ("\033[1;31m");
		break;
	case d_purple:
		printf ("\033[0;35m");
		break;
	case d_light_purple:
		printf ("\033[1;35m");
		break;
	case d_brown:
		printf ("\033[0;33m");
		break;
	case d_yellow:
		printf ("\033[1;33m");
		break;
	case d_light_gray:
		printf ("\033[0;37m");
		break;
	case d_white:
		printf ("\033[1;37m");
		break;
	default:
		d_bug ("Invalid terminal color");
	}
}

void d_gui_terminal_set_bg_color (enum d_gui_terminal_color color) {
	switch (color) {
	case d_black:
		break;
	case d_dark_gray:
		break;
	case d_blue:
		break;
	case d_light_blue:
		break;
	case d_green:
		break;
	case d_light_green:
		break;
	case d_cyan:
		break;
	case d_light_cyan:
		break;
	case d_red:
		break;
	case d_light_red:
		break;
	case d_purple:
		break;
	case d_light_purple:
		break;
	case d_brown:
		break;
	case d_yellow:
		break;
	case d_light_gray:
		break;
	case d_white:
		break;
	default:
		d_bug ("Invalid terminal color");
	}
}

void
d_gui_terminal_step () {
	d_gui_terminal_update_size ();
	d_gui_terminal_hide_cursor ();

	int center = d_gui_terminal_size.width / 2;

	d_gui_terminal_set_text_color (d_green);
	for (int i=0;i<7;++i) {
		d_gui_terminal_printf_center (center, 3 + i, d_gui_terminal_game_title[i]);
	}

	d_gui_terminal_set_text_color (d_light_gray);
	d_gui_terminal_printf_center (center, 13, "New game");
	d_gui_terminal_printf_center (center, 14, "Continue");
	d_gui_terminal_printf_center (center, 15, "Explore");
	d_gui_terminal_printf_center (center, 16, "Quit");
}

void
d_gui_terminal_run () {
	while (1) {
		d_gui_terminal_step ();
	}
}
