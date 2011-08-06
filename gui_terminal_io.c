#include <errno.h>
#include <fcntl.h>
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
static struct termios d_gui_terminal_orig_tty;

void (*d_gui_terminal_update) (double now, double delta) = 0;
void (*d_gui_terminal_draw) () = 0;
void (*d_gui_terminal_key) (char key) = 0;

struct d_gui_terminal_size d_gui_terminal_size;

void
d_gui_terminal_box (int x1, int y1, int x2, int y2, char key) {
	d_gui_terminal_setpos (x1, y1);
	for (int i=x1;i<=x2;++i) {
		printf ("%c", key);
	}

	for (int i=y1+1;i<y2;++i) {
		d_gui_terminal_setpos (x1, i);
		printf ("%c", key);
		d_gui_terminal_setpos (x2, i);
		printf ("%c", key);
	}

	d_gui_terminal_setpos (x1, y2);
	for (int i=x1;i<=x2;++i) {
		printf ("%c", key);
	}
}

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
d_gui_terminal_printf_center (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_gui_terminal_setpos (x - (width / 2), y);
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
}

void
d_gui_terminal_printf_left (int x, int y, const char *format, ...) {
	d_gui_terminal_setpos (x, y);
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
}

void
d_gui_terminal_printf_right (int x, int y, const char *format, ...) {
	int width = strlen (format);
	d_gui_terminal_setpos (x - (width), y);
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
}

void
d_gui_terminal_hide_cursor () {
	printf ("\033[?25l");
}

void
d_gui_terminal_show_cursor () {
	printf ("\033[?25h");
}

void d_gui_terminal_set_color (enum d_gui_terminal_color bg, enum d_gui_terminal_color text) {
	printf ("\033[");
	switch (text) {
	case d_black:
		printf ("0;30");
		break;
	case d_dark_gray:
		printf ("1;30");
		break;
	case d_red:
		printf ("0;31");
		break;
	case d_light_red:
		printf ("1;31");
		break;
	case d_green:
		printf ("0;32");
		break;
	case d_light_green:
		printf ("1;32");
		break;
	case d_brown:
		printf ("0;33");
		break;
	case d_yellow:
		printf ("1;33");
		break;
	case d_blue:
		printf ("0;34");
		break;
	case d_light_blue:
		printf ("1;34");
		break;
	case d_purple:
		printf ("0;35");
		break;
	case d_light_purple:
		printf ("1;35");
		break;
	case d_cyan:
		printf ("0;36");
		break;
	case d_light_cyan:
		printf ("1;36");
		break;
	case d_light_gray:
		printf ("0;37");
		break;
	case d_white:
		printf ("1;37");
		break;
	default:
		d_bug ("Invalid terminal color");
	}

	printf (";");

	switch (bg) {
	case d_none:
		printf ("0m");
		break;
	case d_black:
		printf ("40m");
		break;
	case d_red:
		printf ("41m");
		break;
	case d_green:
		printf ("42m");
		break;
	case d_yellow:
		printf ("43m");
		break;
	case d_blue:
		printf ("44m");
		break;
	case d_purple:
		printf ("45m");
		break;
	case d_cyan:
		printf ("46m");
		break;
	case d_white:
		printf ("47m");
		break;
	default:
		d_bug ("Invalid terminal color");
	}
}

void
d_gui_terminal_tty_raw (int fd) {
    struct termios raw;

    raw = d_gui_terminal_orig_tty;  /* copy original and then modify below */

    /* input modes - clear indicated ones giving: no break, no CR to NL,
       no parity check, no strip char, no start/stop output (sic) control */
    raw.c_iflag &= ~(BRKINT |ICRNL | INPCK | ISTRIP | IXON);

    /* output modes - clear giving: no post processing such as NL to CR+NL */
    raw.c_oflag &= ~(OPOST);

    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);

    /* local modes - clear giving: echoing off, canonical off (no erase with
       backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    /* control chars - set return condition: min number of bytes and timer */
    raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 0; /* immediate - anything       */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd, TCSAFLUSH, &raw) < 0) {
		d_bug ("Cannot set raw tty mode.");
	}
}

void
d_gui_terminal_tty_reset (int fd) {
    if (tcsetattr(fd, TCSAFLUSH, &d_gui_terminal_orig_tty) < 0) {
		d_bug ("failed to reset tty.");
	}
}

void
d_gui_terminal_process_input (int fd) {
	char buffer[10];
	ssize_t result = read (fd, buffer, 10);
	if (result == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
	}
	for (int i=0;i<result;++i) {
		if (d_gui_terminal_key) {
			d_gui_terminal_key (buffer[i]);
		}
	}
}

void
d_gui_terminal_step (double now, double delta) {
	if (d_gui_terminal_update) {
		d_gui_terminal_update (now, delta);
	}

	if (now - d_redraw_last < d_redraw_interval) {
		return;
	}
	if (d_gui_terminal_draw) {
		d_gui_terminal_update_size ();

		d_gui_terminal_draw ();

		d_redraw_last = now;
		fflush(stdout);
	}
}

void
d_gui_terminal_run () {
	int flags = fcntl (STDIN_FILENO, F_GETFL, 0);
	if (fcntl (STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
		d_bug ("Failed to reconfigure stdin to nonblocking.");
	}

	if (tcgetattr(STDIN_FILENO, &d_gui_terminal_orig_tty) < 0) {
		d_bug ("Failed to obtain current tty settings.");
	}

	d_gui_terminal_tty_raw (STDIN_FILENO);

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

	d_gui_terminal_tty_reset (STDIN_FILENO);
	d_gui_terminal_clear ();
	d_gui_terminal_setpos (0, 0);
	d_gui_terminal_show_cursor ();
}

void
d_gui_terminal_sigint (int sig) {
	d_quit = 1;
}
