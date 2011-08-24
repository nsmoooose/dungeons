#ifndef __DUNGEONS_OBJECT_H__
#define __DUNGEONS_OBJECT_H__

struct d_ob_instantce;

struct d_ob_state {
	char *description;
	void (*enter) (struct d_ob_instantce *inst, struct d_ob_state *state);
	void (*input) (struct d_ob_instantce *inst, struct d_ob_state *state);
	void (*exit) (struct d_ob_instantce *inst, struct d_ob_state *state);
};

struct d_ob_state_transition {
	char *description;
	struct d_ob_state *from, *to;

	void (*transition) (
		struct d_ob_instantce *inst,
		struct d_ob_state *from,
		struct d_ob_state *to);
};

struct d_ob_state_machine {
	char *description;
	struct d_ob_state *start;
	struct d_ob_state *states[10];
	struct d_ob_state_transition* transitions[10];
};

struct d_ob_type {
	char *description;
	struct d_ob_state_machine *sm;
	void *data;

	struct d_ob_instance *(*create) ();
	void (*destroy) (struct d_ob_instance *inst);
	void (*serialize) (struct d_ob_type *type, struct d_ob_instance *inst);
};

struct d_ob_instance {
	struct d_ob_type *type;
	struct d_ob_state *state;
	void *data;
};

struct d_ob_category {
	char *description;
	struct d_ob_type* objects[50];
};

struct d_ob_registry {
	struct d_ob_category* categories[50];
};

extern struct d_ob_registry d_ob_registry;

#endif
