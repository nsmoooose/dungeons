#ifndef __DUNGEONS_OBJECT_H__
#define __DUNGEONS_OBJECT_H__

#include "game.h"
#include "storage.h"

struct d_ob_instance;
struct d_ob_state_transition;

struct d_ob_pos {
	int x,y,z;
};

struct d_ob_state {
	char *id;
	void (*enter) (struct d_game_context *context, struct d_ob_instance *inst, struct d_ob_state_transition *transition);
	void (*input) (struct d_game_context *context, struct d_ob_instance *inst);
	void (*exit) (struct d_game_context *context, struct d_ob_instance *inst, struct d_ob_state_transition *transition);
};

struct d_ob_state_transition {
	char *description;
	struct d_ob_state *from, *to;

	void (*transition) (
		struct d_ob_instance *inst,
		struct d_ob_state *from,
		struct d_ob_state *to);
};

struct d_ob_state_machine {
	char *description;
	struct d_ob_state *start;
	struct d_ob_state *states[10];
	struct d_ob_state_transition* transitions[10];
};

enum d_ob_serialize_mode {
	d_ob_write,
	d_ob_read
};

struct d_ob_type {
	char *id;
	char *description;
	struct d_ob_state_machine *sm;
	void *data;

	struct d_ob_instance *(*create) (struct d_ob_type *type, int x, int y, int z);
	void (*destroy) (struct d_ob_instance *inst);
	void (*serialize) (struct d_ob_instance *inst, struct d_storage *storage, enum d_ob_serialize_mode mode);
};

struct d_ob_instance {
	struct d_ob_type *type;
	struct d_ob_state *state;
	struct d_ob_pos pos;
	void *data;
};

struct d_ob_category {
	char *description;
	struct d_ob_type* objects[50];
};

struct d_ob_registry {
	struct d_ob_category* categories[50];
};

struct d_list *d_ob_list_new ();
struct d_ob_type *d_ob_get_type (struct d_ob_registry *registry, const char *id);
struct d_ob_state *d_ob_get_state (struct d_ob_state_machine *sm, const char *id);

extern struct d_ob_registry d_ob_registry;

#endif
