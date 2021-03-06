#ifndef __DUNGEONS_OBJECT_H__
#define __DUNGEONS_OBJECT_H__

#include "htable.h"
#include "math.h"
#include "storage.h"

/* ******************************************************************************** */
/*                               STATE MACHINE                                      */
/* ******************************************************************************** */
struct d_ob_instance;
struct d_ob_state_transition;

struct d_ob_state {
	char *id;
	void (*enter) (struct d_ob_instance *inst, struct d_ob_state_transition *transition);
	void (*input) (struct d_ob_instance *inst);
	void (*exit) (struct d_ob_instance *inst, struct d_ob_state_transition *transition);
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

/* ******************************************************************************** */
/*                               PROPERTIES                                         */
/* ******************************************************************************** */
struct d_prop_instance;

typedef void (*d_ob_property_change_cb) (struct d_ob_instance *instance,
                                         struct d_prop_instance *property);

struct d_prop_type {
	char *id;
	struct d_prop_instance *(*create) (struct d_prop_type *type);
	void (*destroy) (struct d_prop_instance *inst);
	void (*serialize) (struct d_storage *storage, struct d_prop_instance *inst);
	void (*deserialize) (struct d_storage *storage, struct d_prop_instance *inst);
};

struct d_prop_def {
	char *id;
	struct d_prop_type *type;
};

struct d_prop_instance {
	struct d_prop_type *type;
	struct d_list *change;
	void *value;
};

float d_prop_float_get (struct d_prop_instance *inst);
void d_prop_float_set (struct d_prop_instance *inst, float value);
extern struct d_prop_type d_prop_float;

int d_prop_int_get (struct d_prop_instance *inst);
void d_prop_int_set (struct d_prop_instance *inst, int value);
extern struct d_prop_type d_prop_int;

struct d_point3 *d_prop_pos3_get (struct d_prop_instance *inst);
extern struct d_prop_type d_prop_pos3;

char *d_prop_str_get (struct d_prop_instance *inst);
void d_prop_str_set (struct d_prop_instance *inst, const char *value);
extern struct d_prop_type d_prop_str;

/* ******************************************************************************** */
/*                               OBJECTS                                            */
/* ******************************************************************************** */

struct d_ob_type {
	char *id;
	char *description;
	struct d_ob_state_machine *sm;
	void *data;
	struct d_prop_def* properties[10];

	struct d_ob_instance *(*create) (struct d_ob_type *type);
	void (*destroy) (struct d_ob_instance *inst);
	void (*serialize) (struct d_ob_instance *inst, struct d_storage *storage,
					   enum d_ob_serialize_mode mode);
};

struct d_ob_instance {
	struct d_ob_type *type;
	struct d_ob_state *state;
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
void d_ob_do_transition (struct d_ob_instance *instance,
                         struct d_ob_state_transition *transition);

struct d_htable *d_ob_property_htable_new (int size);
struct d_prop_instance *d_prop_instance_new (
  struct d_htable *properties, struct d_prop_def *def);
int d_ob_property_value_int_get (struct d_prop_instance *instance);
float d_ob_property_value_float_get (struct d_prop_instance *instance);
char* d_ob_property_value_str_get (struct d_prop_instance *instance);

extern struct d_ob_registry d_ob_registry;

#endif
