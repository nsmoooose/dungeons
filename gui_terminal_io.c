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
#include "gui_terminal_io.h"
#include "gui_terminal_main_menu.h"

double d_redraw_last = 0.0;
double d_redraw_interval = 1.0;
int d_quit = 0;
int d_request_redraw = 0;

void (*d_gui_terminal_update) (double now, double delta) = 0;
void (*d_gui_terminal_draw) () = 0;
void (*d_gui_terminal_key) (char key) = 0;

struct d_gui_terminal_size d_gui_terminal_size;

struct d_color_def {
	enum d_gui_terminal_color index;
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
	{ d_cyan_white, COLOR_CYAN, COLOR_WHITE }
};

void
d_gui_terminal_box (int x1, int y1, int x2, int y2, char key) {
	d_gui_terminal_setpos (x1, y1);
	for (int i=x1;i<=x2;++i) {
		printw ("%c", key);
	}

	for (int i=y1+1;i<y2;++i) {
		d_gui_terminal_setpos (x1, i);
		printw ("%c", key);
		d_gui_terminal_setpos (x2, i);
		printw ("%c", key);
	}

	d_gui_terminal_setpos (x1, y2);
	for (int i=x1;i<=x2;++i) {
		printw ("%c", key);
	}
}

void
d_gui_terminal_update_size () {
	getmaxyx (stdscr, d_gui_terminal_size.height, d_gui_terminal_size.width);
}

void
d_gui_terminal_clear () {
	if (erase () == ERR) {
		d_bug ("Failed to erase screen.");
	}
}

void
d_gui_terminal_setpos (int x, int y) {
	if (move (y, x) == ERR) {
		d_bug ("Failed to move cursor position.");
	}
}

void
d_gui_terminal_printf_center (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_gui_terminal_setpos (fmax (x - (width / 2), 0), y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

void
d_gui_terminal_printf_left (int x, int y, const char *format, ...) {
	d_gui_terminal_setpos (x, y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

void
d_gui_terminal_printf_right (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_gui_terminal_setpos (x - (width), y);
	va_list args;
	va_start (args, format);
	vwprintw (stdscr, format, args);
	va_end (args);
}

void
d_gui_terminal_hide_cursor () {
	if (curs_set (0) == ERR) {
		d_bug ("Failed to hide cursor");
	}
}

void
d_gui_terminal_show_cursor () {
	if (curs_set (1) == ERR) {
		d_bug ("Failed to show cursor");
	}
}

void d_gui_terminal_set_color (enum d_gui_terminal_color pair) {
	if (color_set (pair, 0) == ERR) {
		d_bug ("Failed to set color");
	}
}

void
d_gui_terminal_request_redraw () {
	d_request_redraw = 1;
}

void
d_gui_terminal_process_input (int fd) {
	int key = getch ();
	if (key == ERR) {
		return;
	}
	if (d_gui_terminal_key) {
		d_gui_terminal_key (key);
		d_gui_terminal_request_redraw ();
	}
}

void
d_gui_terminal_step (double now, double delta) {
	if (d_gui_terminal_update) {
		d_gui_terminal_update (now, delta);
	}

	if (d_request_redraw == 0 && now - d_redraw_last < d_redraw_interval) {
		return;
	}
	if (d_gui_terminal_draw) {
		d_request_redraw = 0;
		d_gui_terminal_update_size ();

		d_gui_terminal_draw ();

		d_redraw_last = now;
		refresh ();
	}
}

void
d_gui_terminal_run () {
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

	signal (SIGINT, d_gui_terminal_sigint);

	d_gui_terminal_draw = d_gui_terminal_main_menu_draw;
	d_gui_terminal_update = d_gui_terminal_main_menu_update;
	d_gui_terminal_key = d_gui_terminal_main_menu_key;

	double last = 0;

	d_gui_terminal_clear ();
	d_gui_terminal_hide_cursor ();

	while (!d_quit) {
		double now = d_time_get ();
		d_gui_terminal_process_input (STDIN_FILENO);
		d_gui_terminal_step (now, last-now);
	}

	d_gui_terminal_clear ();
	d_gui_terminal_setpos (0, 0);
	d_gui_terminal_show_cursor ();

	if (endwin () == ERR) {
		d_bug ("Failed to end ncurses.");
	}
}

void
d_gui_terminal_sigint (int sig) {
	d_quit = 1;
}
