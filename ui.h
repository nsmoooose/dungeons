#ifndef __DUNGEONS_UI_H__
#define __DUNGEONS_UI_H__

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

enum d_ui_map_mode {
	/* Draws like a regular map where we can see information from all heights. */
	d_map,
	/* Draw a slice from the current viewpoint. You see through the earth and. */
	d_level,
};

struct d_ui_viewpoint {
	int x, y, z;
};

extern struct d_ui_state *d_ui_state_current;

#endif
