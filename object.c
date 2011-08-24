#include "object.h"
#include "object_tree.h"

struct d_ob_registry d_ob_registry = {
	{
		&d_ob_category_trees,
		0
	}
};
