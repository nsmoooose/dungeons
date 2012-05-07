#include "math.h"
#include "memory.h"
#include "octree.h"

struct d_octree_node *
d_octree_node_new (int x, int y, int z, int half_dimension) {
	struct d_octree_node *node = d_calloc (1, sizeof (struct d_octree_node));
	node->pos.x = x;
	node->pos.y = y;
	node->pos.z = z;
	node->half_dimension = half_dimension;
	return node;
}

struct d_octree *
d_octree_new (int capacity, int half_dimension) {
	if (half_dimension <= 1 || !d_is_power_of_2 (half_dimension)) {
		return 0;
	}
	struct d_octree *tree = d_calloc (1, sizeof (struct d_octree));
	tree->capacity = capacity;
	tree->root = d_octree_node_new (0, 0, 0, half_dimension);
	return tree;
}

void
d_octree_free (struct d_octree *tree) {
	d_free (tree->root);
	d_free (tree);
}

struct d_octree_obj *
d_octree_insert (struct d_octree *tree, int x, int y, int z,
                 struct d_ob_instance *instance) {
	return 0;
}

void
d_octree_delete (struct d_octree *tree, struct d_octree_obj *object) {
}

void
d_octree_traverse (struct d_octree *tree, d_octree_cb cb) {
}

void
d_octree_traverse_aabb (struct d_octree *tree, int x1, int y1, int x2, int y2, d_octree_cb cb) {
}
