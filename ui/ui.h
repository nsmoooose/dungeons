#ifndef __DUNGEONS_UI_H__
#define __DUNGEONS_UI_H__

#include "fractal_heightmap.h"

struct d_ui_pos {
	int x, y;
};

struct d_ui_size {
	int width, height;
};

struct d_ui_area {
	struct d_ui_pos pos;
	struct d_ui_size size;
};

struct d_ui_command {
	char *description;
	void (*invoke) ();
};

struct d_ui_key_binding {
	int key;
	struct d_ui_command *command;
};

struct d_ui_state {
	char *description;
	void *data;
	void (*update) (struct d_ui_state *handler, double now, double delta);
	void (*draw) (struct d_ui_state *handler);

	struct d_ui_key_binding key_bindings[100];
};

struct d_ui_state_transition {
	char *description;
	struct d_ui_state *from;
	struct d_ui_state *next;
};

struct d_ui_state_machine {
	char *description;
	struct d_ui_state *current;
	struct d_ui_state* states[100];
	struct d_ui_state_transition* transitions[100];
};

enum d_ui_color {
	d_black_white=1,
	d_white_black,
	d_black_green,
	d_green_white,
	d_cyan_white,
	d_black_blue,
	d_black_cyan
};

struct d_ui_viewpoint {
	int x, y, z;
};

struct d_ui {
	int quit;

	struct d_ui_size size;

	double redraw_last;
	double redraw_interval;
	int request_redraw;

	void (*init) ();
	void (*destroy) ();

	void (*clearscr) ();
	void (*setpos) (int x, int y);
	void (*printf_center) (int x, int y, const char *format, ...);
	void (*printf_left) (int x, int y, const char *format, ...);
	void (*printf_right) (int x, int y, const char *format, ...);
	void (*box) (int x1, int y1, int x2, int y2, char key);
	void (*update_size) ();
	void (*set_color) (enum d_ui_color pair);
	void (*hide_cursor) ();
	void (*show_cursor) ();

	void (*title_large_draw) ();
	void (*menu_draw) (struct d_ui_area *area, struct d_ui_key_binding menu[]);

	void (*map_draw) (struct d_ui_area *area, struct d_ui_viewpoint *vp,
					  int zoom, struct d_heightmap *hm);

};

void d_ui_do_transition (struct d_ui_state_transition *transition);
struct d_ui_state* d_ui_state_current ();
void d_ui_state_machine_print (char *filename);

void d_ui_run ();

enum d_ui_map_mode {
	/* Draws like a regular map where we can see information from all heights. */
	d_map,
	/* Draw a slice from the current viewpoint. You see through the earth. */
	d_level,
};

extern struct d_ui *d_ui;

extern struct d_ui_state_transition d_transition_new_game;
extern struct d_ui_state_transition d_transition_new_world;
extern struct d_ui_state_transition d_transition_new_world_back;
extern struct d_ui_state_transition d_transition_quit_question;
extern struct d_ui_state_transition d_transition_quit_resume;

extern struct d_ui_state_transition d_transition_esc_menu;
extern struct d_ui_state_transition d_transition_esc_menu_resume;
extern struct d_ui_state_transition d_transition_esc_menu_to_main_menu;
#endif
