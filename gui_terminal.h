#ifndef __DUNGEONS_GUI_TERMINAL_H__
#define __DUNGEONS_GUI_TERMINAL_H__

enum d_gui_terminal_style {
	d_normal,
	d_bold,
	d_italic
};

enum d_gui_terminal_color {
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

void d_gui_terminal_update_size ();
void d_gui_terminal_clear ();
void d_gui_terminal_setpos (int x, int y);
void d_gui_terminal_printf_center (int x, int y, const char *format);
void d_gui_terminal_hide_cursor ();
void d_gui_terminal_show_cursor ();
void d_gui_terminal_set_style (enum d_gui_terminal_style style);
void d_gui_terminal_set_text_color (enum d_gui_terminal_color color);
void d_gui_terminal_set_bg_color (enum d_gui_terminal_color color);

void d_gui_terminal_step ();
void d_gui_terminal_run ();

#endif
