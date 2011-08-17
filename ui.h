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

struct d_ui_handler {
	void *data;
	void (*update) (struct d_ui_handler *handler, double now, double delta);
	void (*draw) (struct d_ui_handler *handler);
	void (*input) (struct d_ui_handler *handler, int key);
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

struct d_ui_handler* d_ui_stack_pop ();
void d_ui_stack_push (struct d_ui_handler *handler);
struct d_ui_handler* d_ui_stack_current ();
struct d_ui_handler* d_ui_stack_prev (struct d_ui_handler *handler);

#endif
