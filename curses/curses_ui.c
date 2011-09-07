#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "clock.h"
#include "error.h"
#include "curses_ui.h"
#include "main_menu.h"
#include "ui.h"

struct d_color_def {
	enum d_ui_color index;
	int bg;
	int text;
};

/*
  COLOR_BLACK
  COLOR_RED
  COLOR_GREEN
  COLOR_YELLOW
  COLOR_BLUE
  COLOR_MAGENTA
  COLOR_CYAN
  COLOR_WHITE
*/

struct d_color_def d_color_def[] =
{
	{ d_black_white, COLOR_BLACK, COLOR_WHITE },
	{ d_white_black, COLOR_WHITE, COLOR_BLACK },
	{ d_black_green, COLOR_BLACK, COLOR_GREEN },
	{ d_green_white, COLOR_GREEN, COLOR_WHITE },
	{ d_cyan_white, COLOR_CYAN, COLOR_WHITE },
	{ d_black_blue, COLOR_BLACK, COLOR_BLUE },
	{ d_black_cyan, COLOR_BLACK, COLOR_CYAN }
};

static void
d_curses_box (int x1, int y1, int x2, int y2, char key) {
	d_ui->setpos (x1, y1);
	for (int i=x1;i<=x2;++i) {
		printw ("%c", key);
	}

	for (int i=y1+1;i<y2;++i) {
		d_ui->setpos (x1, i);
		printw ("%c", key);
		d_ui->setpos (x2, i);
		printw ("%c", key);
	}

	d_ui->setpos (x1, y2);
	for (int i=x1;i<=x2;++i) {
		printw ("%c", key);
	}
}

static void
d_curses_update_size () {
	getmaxyx (stdscr, d_ui->size.height, d_ui->size.width);
}

static void
d_curses_clear () {
	if (erase () == ERR) {
		d_bug ("Failed to erase screen.");
	}
}

static void
d_curses_setpos (int x, int y) {
	if (move (y, x) == ERR) {
		d_bug ("Failed to move cursor position.");
	}
}

static void
d_curses_printf_center (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_curses_setpos (fmax (x - (width / 2), 0), y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

static void
d_curses_printf_left (int x, int y, const char *format, ...) {
	d_ui->setpos (x, y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

static void
d_curses_printf_right (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_ui->setpos (x - (width), y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

static void
d_curses_hide_cursor () {
	if (curs_set (0) == ERR) {
		d_bug ("Failed to hide cursor");
	}
}

static void
d_curses_show_cursor () {
	if (curs_set (1) == ERR) {
		d_bug ("Failed to show cursor");
	}
}

static void
d_curses_set_color (enum d_ui_color pair) {
	if (color_set (pair, 0) == ERR) {
		d_bug ("Failed to set color");
	}
}

void
d_curses_process_input () {
	int key = getch ();
	if (key == ERR) {
		return;
	}
	struct d_ui_state *state = d_ui_state_current ();
	for (int i=0;state->key_bindings[i].key != 0;++i) {
		if (key == state->key_bindings[i].key) {
			state->key_bindings[i].command->invoke ();
			d_ui->request_redraw = 1;
			break;
		}
	}
}

void
d_curses_step (double now, double delta) {
	struct d_ui_state *state = d_ui_state_current ();
	if (state && state->update) {
		state->update (state, now, delta);
	}

	if (d_ui->request_redraw == 0 && now - d_ui->redraw_last < d_ui->redraw_interval) {
		return;
	}
	if (state) {
		d_ui->request_redraw = 0;
		d_ui->update_size ();

		if (state->draw) {
			state->draw (state);
		}
		else {
			d_ui->clearscr ();

			d_ui->title_large_draw ();

			struct d_ui_area area = { { d_ui->size.width / 2 - 10, 13}, { 20, 20 } };
			d_ui->menu_draw (&area, state->key_bindings);
		}

		d_ui->redraw_last = now;
		refresh ();
	}
}

static void
d_curses_widget_title_large_draw () {
	const static char *title[] = {
		"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
		"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
		"D     D U    U N N  N G      E     O    O  N N  N S     ",
		"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
		"D     D U    U N  N N G    G E     O    O  N  N N      S",
		"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
		"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
	};
	d_ui->set_color (d_black_green);

	int center = d_ui->size.width / 2;

	for (int i=0;i<7;++i) {
		d_ui->printf_center (center, 3 + i, title[i]);
	}
}

static void
d_curses_widget_menu_draw (struct d_ui_area *area, struct d_ui_key_binding menu[]) {
	d_ui->set_color (d_black_white);

	for (int i=0,row=area->pos.y;menu[i].key != 0;++i,++row) {
		d_ui->printf_left (area->pos.x, row, "%c) %s",
						   menu[i].key, menu[i].command->description);
	}
}

static struct d_ui d_curses_implementation = {
	0,          /* quit flag */
	{ 0, 0 },   /* size */
	0.0,        /* last redraw */
	1.0,        /* redraw interval */
	0,          /* request redraw flag */
	d_curses_clear,
	d_curses_setpos,
	d_curses_printf_center,
	d_curses_printf_left,
	d_curses_printf_right,
	d_curses_box,
	d_curses_update_size,
	d_curses_set_color,
	d_curses_hide_cursor,
	d_curses_show_cursor,

	d_curses_widget_title_large_draw,
	d_curses_widget_menu_draw
};

void
d_curses_sigint (int sig) {
	d_ui->quit = 1;
}

void
d_curses_run () {
	d_ui = &d_curses_implementation;

	if (!initscr ()) {
		d_bug ("Failed to initialize ncurses.");
	}

	if (cbreak () == ERR) {
		d_bug ("Failed to put terminal in raw mode.");
	}
	if (noecho () == ERR) {
		d_bug ("Failed to disable echo.");
	}
	if (keypad (stdscr, TRUE) == ERR) {
		d_bug ("Failed to initialize keypad.");
	}
	if (curs_set (0) == ERR) {
		d_bug ("Failed to hide cursor.");
	}
	if (start_color () == ERR) {
		d_bug ("Failed to initialize colors.");
	}
	if (nodelay (stdscr, TRUE) == ERR) {
		d_bug ("Failed to set nodelay.");
	}

	for (int i=0;i<sizeof (d_color_def) / sizeof (struct d_color_def);++i) {
		if (init_pair (d_color_def[i].index, d_color_def[i].text, d_color_def[i].bg) == ERR) {
			d_bug ("Failed to set color");
		}
	}

	signal (SIGINT, d_curses_sigint);

	double last = 0;

	d_ui->clearscr ();
	d_ui->hide_cursor ();

	while (!d_ui->quit) {
		double now = d_time_get ();
		d_curses_process_input ();
		d_curses_step (now, now-last);
		last = now;
	}

	d_ui->clearscr ();
	d_ui->setpos (0, 0);
	d_ui->show_cursor ();

	if (endwin () == ERR) {
		d_bug ("Failed to end ncurses.");
	}
}
