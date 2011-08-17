#ifndef __DUNGEONS_CURSES_IO_H__
#define __DUNGEONS_CURSES_IO_H__

enum d_curses_color {
	d_black_white=1,
	d_white_black,
	d_black_green,
	d_green_white,
	d_cyan_white,
	d_black_blue,
	d_black_cyan
};

extern int d_quit;

extern struct d_curses_size {
	int width;
	int height;
} d_curses_size;

extern void (*d_curses_update) (double now, double delta);
extern void (*d_curses_draw) ();
extern void (*d_curses_key) (char key);

void d_curses_update_size ();
void d_curses_clear ();
void d_curses_setpos (int x, int y);
void d_curses_printf_center (int x, int y, const char *format, ...);
void d_curses_printf_left (int x, int y, const char *format, ...);
void d_curses_printf_right (int x, int y, const char *format, ...);
void d_curses_hide_cursor ();
void d_curses_show_cursor ();
void d_curses_set_color (enum d_curses_color pair);
void d_curses_sigint (int sig);
void d_curses_box (int x1, int y1, int x2, int y2, char key);
void d_curses_request_redraw ();

void d_curses_step (double now, double delta);
void d_curses_run ();

#endif
