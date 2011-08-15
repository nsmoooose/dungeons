#ifndef __DUNGEONS_GUI_TERMINAL_IO_H__
#define __DUNGEONS_GUI_TERMINAL_IO_H__

enum d_gui_terminal_color {
	d_black_white=1,
	d_white_black,
	d_black_green,
	d_green_white,
	d_cyan_white,
	d_black_blue
};

extern int d_quit;

extern struct d_gui_terminal_size {
	int width;
	int height;
} d_gui_terminal_size;

struct d_command {
	char *description;
	void (*invoke) ();
};

struct d_key_binding {
	int key;
	struct d_command *command;
};

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
void d_gui_terminal_set_color (enum d_gui_terminal_color pair);
void d_gui_terminal_sigint (int sig);
void d_gui_terminal_box (int x1, int y1, int x2, int y2, char key);
void d_gui_terminal_request_redraw ();

void d_gui_terminal_step (double now, double delta);
void d_gui_terminal_run ();

#endif
