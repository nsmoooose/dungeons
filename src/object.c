#include <string.h>

#include "error.h"
#include "list.h"
#include "memory.h"
#include "object.h"
#include "object_tree.h"

static void
d_ob_remove (void *data) {
	struct d_ob_instance *inst = data;
	inst->type->destroy (inst);
}

struct d_list*
d_ob_list_new () {
	struct d_list *list = d_list_new ();
	list->remove = d_ob_remove;
	return list;
}

struct d_ob_type *
d_ob_get_type (struct d_ob_registry *registry, const char *id) {
	for (int i=0;registry->categories[i];++i) {
		struct d_ob_category *category = registry->categories[i];
		for (int j=0;category->objects[j];++j) {
			struct d_ob_type *type = category->objects[j];
			if (strcmp (type->id, id) == 0) {
				return type;
			}
		}
	}
	d_bug ("Object type: %s not found.", id);
	return 0;
}

struct d_ob_state *
d_ob_get_state (struct d_ob_state_machine *sm, const char *id) {
	for (int i=0;sm->states[i];++i) {
		struct d_ob_state *state = sm->states[i];
		if (strcmp (state->id, id) == 0) {
			return state;
		}
	}
	d_bug ("Object state not found: %s", id);
	return 0;
}

void
d_ob_do_transition (struct d_game_context *context,
					struct d_ob_instance *instance,
					struct d_ob_state_transition *transition) {
	struct d_ob_state_machine *sm = instance->type->sm;
	for (int i=0;sm->transitions[i];++i) {
		if (sm->transitions[i] == transition) {
			if (instance->state == transition->from) {
				if (instance->state->exit) {
					instance->state->exit (context, instance, transition);
				}
				if (transition->to->enter) {
					transition->to->enter (context, instance, transition);
				}
				instance->state = transition->to;
				return;
			}
			else {
				d_bug ("Illegal transition (%s) when object (%s) is in state (%s)", transition->description, instance->type->id, sm->description);
			}
		}
	}
	d_bug ("Transition (%s) not found in state machine (%s)",
		   transition->description, sm->description);
}

static void
d_ob_property_instance_htable_remove (void *key, void *value) {
	d_free (value);
}

struct d_htable *
d_ob_property_htable_new (int size) {
	struct d_htable *properties = d_htable_new_str (size);
	properties->remove = d_ob_property_instance_htable_remove;
	return properties;
}

struct d_ob_property_instance *
d_ob_property_instance_new (struct d_htable *properties,
							struct d_ob_property_type *type) {
	struct d_ob_property_instance *instance = d_calloc
		(1, sizeof (struct d_ob_property_instance));
	instance->type = type;
	d_htable_insert (properties, type->id, instance);
	return instance;
}

void
d_ob_property_write (struct d_storage *storage, struct d_ob_property_instance *instance) {
	switch (instance->type->data_type) {
	case d_string:
		d_storage_write_s (storage, instance->value.str_v);
		break;
	case d_double:
		d_storage_write_d (storage, &instance->value.double_v);
		break;
	case d_float:
		d_storage_write_f (storage, &instance->value.float_v);
		break;
	case d_int:
		d_storage_write_i (storage, &instance->value.int_v);
		break;
	default:
		d_bug ("Unknown type of property");
	}
}

void
d_ob_property_read (struct d_storage *storage, struct d_ob_property_instance *instance) {
	switch (instance->type->data_type) {
	case d_string:
		instance->value.str_v = d_storage_read_s (storage);
		break;
	case d_double:
		d_storage_read_d (storage, &instance->value.double_v);
		break;
	case d_float:
		d_storage_read_f (storage, &instance->value.float_v);
		break;
	case d_int:
		d_storage_read_i (storage, &instance->value.int_v);
		break;
	default:
		d_bug ("Unknown type of property");
	}
}

struct d_ob_registry d_ob_registry = {
	{
		&d_ob_category_trees,
		0
	}
};

