#include <string.h>

#include "error.h"
#include "list.h"
#include "memory.h"
#include "object.h"
#include "object_tree.h"
#include "str.h"

static struct d_prop_instance *
d_prop_point3_create (struct d_prop_type *type) {
	struct d_prop_instance *inst = d_calloc (1, sizeof (struct d_prop_instance));
	inst->type = type;
	inst->value = d_calloc (1, sizeof (struct d_point3));
	return inst;
}

static void
d_prop_destroy (struct d_prop_instance *inst) {
	if (inst) {
		if (inst->value) {
			d_free (inst->value);
		}
		d_free (inst);
	}
}

struct d_prop_type d_prop_pos3 = {
	.create = d_prop_point3_create,
	.destroy = d_prop_destroy
};

struct d_point3 *
d_prop_pos3_get (struct d_prop_instance *inst) {
	if (inst->type != &d_prop_pos3) {
		d_bug (d_invalid_property_usage);
	}
	return (struct d_point3*)inst->value;
}

static struct d_prop_instance *
d_prop_str_create (struct d_prop_type *type) {
	struct d_prop_instance *inst = d_calloc (1, sizeof (struct d_prop_instance));
	inst->type = type;
	return inst;
}

char *
d_prop_str_get (struct d_prop_instance *inst) {
	if (inst->type != &d_prop_str) {
		d_bug (d_invalid_property_usage);
	}
	return inst->value;
}

void
d_prop_str_set (struct d_prop_instance *inst, const char *value) {
	if (inst->type != &d_prop_str) {
		d_bug (d_invalid_property_usage);
	}
	if (inst->value) {
		d_strfree (inst->value);
	}
	inst->value = d_strdup (value);
}

struct d_prop_type d_prop_str = {
	.create = d_prop_str_create,
	.destroy = d_prop_destroy
};

static struct d_prop_instance *d_prop_int_create (struct d_prop_type *type) {
	struct d_prop_instance *inst = d_calloc (1, sizeof (struct d_prop_instance));
	inst->type = type;
	inst->value = d_calloc (1, sizeof (int));
	return inst;
}

struct d_prop_type d_prop_int = {
	.create = d_prop_int_create,
	.destroy = d_prop_destroy
};

static struct d_prop_instance *d_prop_float_create (struct d_prop_type *type) {
	struct d_prop_instance *inst = d_calloc (1, sizeof (struct d_prop_instance));
	inst->type = type;
	inst->value = d_calloc (1, sizeof (float));
	return inst;
}

int
d_prop_int_get (struct d_prop_instance *inst) {
	if (inst->type != &d_prop_int) {
		d_bug (d_invalid_property_usage);
	}
	return *((int*)inst->value);
}

void
d_prop_int_set (struct d_prop_instance *inst, int value) {
	if (inst->type != &d_prop_int) {
		d_bug (d_invalid_property_usage);
	}
	*((int*)inst->value) = value;
}

struct d_prop_type d_prop_float = {
	.create = d_prop_float_create,
	.destroy = d_prop_destroy
};

float
d_prop_float_get (struct d_prop_instance *inst) {
	if (inst->type != &d_prop_float) {
		d_bug (d_invalid_property_usage);
	}
	return *((float*)inst->value);
}

void
d_prop_float_set (struct d_prop_instance *inst, float value) {
	if (inst->type != &d_prop_float) {
		d_bug (d_invalid_property_usage);
	}
	*((float*)inst->value) = value;
}

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
d_ob_do_transition (struct d_ob_instance *instance,
                    struct d_ob_state_transition *transition) {
	struct d_ob_state_machine *sm = instance->type->sm;
	for (int i=0;sm->transitions[i];++i) {
		if (sm->transitions[i] == transition) {
			if (instance->state == transition->from) {
				if (instance->state->exit) {
					instance->state->exit (instance, transition);
				}
				if (transition->to->enter) {
					transition->to->enter (instance, transition);
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
d_prop_instance_htable_remove (void *key, void *value) {
	d_free (value);
}

struct d_htable *
d_ob_property_htable_new (int size) {
	struct d_htable *properties = d_htable_new_str (size);
	properties->remove = d_prop_instance_htable_remove;
	return properties;
}

struct d_prop_instance *
d_prop_instance_new (struct d_htable *properties,
                     struct d_prop_def *def) {
	struct d_prop_instance *instance = def->type->create (def->type);
	d_htable_insert (properties, def->id, instance);
	return instance;
}

void
d_ob_property_write (struct d_storage *storage, struct d_prop_instance *instance) {
	switch (instance->type->data_type) {
	case d_string:
		d_storage_write_s (storage, instance->value);
		break;
	case d_float:
		d_storage_write_f (storage, (float*)instance->value);
		break;
	case d_int:
		d_storage_write_i (storage, (int*)instance->value);
		break;
	default:
		d_bug ("Unknown type of property");
	}
}

void
d_ob_property_read (struct d_storage *storage, struct d_prop_instance *instance) {
	switch (instance->type->data_type) {
	case d_string:
		instance->value = d_storage_read_s (storage);
		break;
	case d_float:
		d_storage_read_f (storage, (float*)instance->value);
		break;
	case d_int:
		d_storage_read_i (storage, (int*)instance->value);
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

