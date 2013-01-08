#include "o_tree.hpp"

world_object *tree_type::create_instance () {
	struct tree_object *tree = new tree_object ();
	tree->type = this;
	return tree;
}

world_object_type *tree_object::get_type () {
	return type;
}

void tree_object::update (double d) {
	/* TODO tree should slowly die outside its normal habitat.
	/* Tree should die outside of its normal habitat.
	 * It would be better to slowly die than to just die quickly. But
	 * this can be an improvement for the future. */
	if (pos->z > type->max_height || pos->z < type->min_height) {
		state = dead;
		return;
	}
	age += d;
	height += (type->growth_rate / 365.0 * d);
}
