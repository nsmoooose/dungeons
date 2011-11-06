#ifndef __DUNGEONS_OBJECT_H__
#define __DUNGEONS_OBJECT_H__

#include "game.h"
#include "htable.h"
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

struct d_ob_instance;
struct d_ob_property_instance;

typedef void (*d_ob_property_change_cb) (struct d_ob_instance *instance,
										 struct d_ob_property_instance *property);

enum d_ob_data_type {
	d_string,
	d_float,
	d_int
};

struct d_ob_property_type {
	char *id;
	enum d_ob_data_type data_type;
};

struct d_ob_property_instance {
	struct d_ob_property_type *type;
	struct d_list *change;
	union {
		int int_v;
		float float_v;
		char *str_v;
	} value;
};

struct d_ob_type {
	char *id;
	char *description;
	struct d_ob_state_machine *sm;
	void *data;
	struct d_ob_property_type* properties[10];

	struct d_ob_instance *(*create) (struct d_ob_type *type, int x, int y, int z);
	void (*destroy) (struct d_ob_instance *inst);
	void (*serialize) (struct d_ob_instance *inst, struct d_storage *storage,
					   enum d_ob_serialize_mode mode);
};

struct d_ob_instance {
	struct d_ob_type *type;
	struct d_ob_state *state;
	struct d_ob_pos pos;
	struct d_htable *properties;
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
struct d_ob_type *d_ob_get_type (struct d_ob_registry *registry,
								 const char *id);
struct d_ob_state *d_ob_get_state (struct d_ob_state_machine *sm,
								   const char *id);
void d_ob_do_transition (struct d_game_context *context,
						 struct d_ob_instance *instance,
						 struct d_ob_state_transition *transition);

struct d_htable *d_ob_property_htable_new (int size);
struct d_ob_property_instance *d_ob_property_instance_new (
  struct d_htable *properties, struct d_ob_property_type *type);
int d_ob_property_value_int_get (struct d_ob_property_instance *instance);
float d_ob_property_value_float_get (struct d_ob_property_instance *instance);
char* d_ob_property_value_str_get (struct d_ob_property_instance *instance);

extern struct d_ob_registry d_ob_registry;

#endif
