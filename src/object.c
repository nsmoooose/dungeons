#include <string.h>

#include "error.h"
#include "list.h"
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

struct d_ob_registry d_ob_registry = {
	{
		&d_ob_category_trees,
		0
	}
};

