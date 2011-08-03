#ifndef __DUNGEONS_GUI_TERMINAL_IO_H__
#define __DUNGEONS_GUI_TERMINAL_IO_H__

enum d_gui_terminal_color {
	d_none,
	d_black,
	d_dark_gray,
	d_blue,
	d_light_blue,
	d_green,
	d_light_green,
	d_cyan,
	d_light_cyan,
	d_red,
	d_light_red,
	d_purple,
	d_light_purple,
	d_brown,
	d_yellow,
	d_light_gray,
	d_white
};

extern int d_quit;

extern struct d_gui_terminal_size {
	int width;
	int height;
} d_gui_terminal_size;

extern void (*d_gui_terminal_update) (double now, double delta);
extern void (*d_gui_terminal_draw) ();
extern void (*d_gui_terminal_key) (char key);

void d_gui_terminal_update_size ();
void d_gui_terminal_clear ();
void d_gui_terminal_setpos (int x, int y);
void d_gui_terminal_printf_center (int x, int y, const char *format, ...);
void d_gui_terminal_printf_left (int x, int y, const char *format, ...);
void d_gui_terminal_printf_right (int x, int y, const char *format, ...);
void d_gui_terminal_hide_cursor ();
void d_gui_terminal_show_cursor ();
void d_gui_terminal_set_color (enum d_gui_terminal_color bg, enum d_gui_terminal_color text);
void d_gui_terminal_sigint (int sig);
void d_gui_terminal_box (int x1, int y1, int x2, int y2, char key);

void d_gui_terminal_step (double now, double delta);
void d_gui_terminal_run ();

#endif
