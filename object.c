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


struct d_ob_registry d_ob_registry = {
	{
		&d_ob_category_trees,
		0
	}
};

