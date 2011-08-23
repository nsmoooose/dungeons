#ifndef __DUNGEONS_UI_H__
#define __DUNGEONS_UI_H__

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

void d_ui_do_transition (struct d_ui_state_transition *transition);
struct d_ui_state* d_ui_state_current ();
void d_ui_state_machine_print (char *filename);

enum d_ui_map_mode {
	/* Draws like a regular map where we can see information from all heights. */
	d_map,
	/* Draw a slice from the current viewpoint. You see through the earth. */
	d_level,
};

struct d_ui_viewpoint {
	int x, y, z;
};

extern struct d_ui_state_transition d_transition_new_game;
extern struct d_ui_state_transition d_transition_new_world;
extern struct d_ui_state_transition d_transition_new_world_back;
extern struct d_ui_state_transition d_transition_quit_question;
extern struct d_ui_state_transition d_transition_quit_resume;

extern struct d_ui_state_transition d_transition_esc_menu;
extern struct d_ui_state_transition d_transition_esc_menu_resume;
extern struct d_ui_state_transition d_transition_esc_menu_to_main_menu;
#endif
