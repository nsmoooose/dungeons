#include <math.h>
#include <string.h>
#include "error.h"
#include "math.h"
#include "memory.h"
#include "octree.h"

static void
d_octree_obj_free (struct d_octree_obj *object) {
	d_free (object);
}

static void
d_octree_node_free (struct d_octree *tree, struct d_octree_node *node) {
	for (struct d_list_node *obj_node=node->objects->first;obj_node;obj_node=obj_node->next) {
		d_octree_obj_free (obj_node->data);
	}
	d_list_destroy (node->objects);
	node->objects = 0;
	for (int i=0;i<8;++i) {
		struct d_octree_node *child_node = node->children[i];
		if (child_node) {
			d_octree_node_free (tree, child_node);
			node->children[i] = 0;
		}
	}
	d_free (node);
	tree->nnodes--;
}

struct d_octree_node *
d_octree_node_new (struct d_aabb3 *aabb) {
	struct d_octree_node *node = d_calloc (1, sizeof (struct d_octree_node));
	memcpy (&node->aabb, aabb, sizeof (struct d_aabb3));
	node->objects = d_list_new ();
	return node;
}

struct d_octree *
d_octree_new (int capacity, int half_dimension) {
	if (half_dimension <= 1 || !d_is_power_of_2 (half_dimension)) {
		return 0;
	}
	struct d_octree *tree = d_calloc (1, sizeof (struct d_octree));
	tree->capacity = capacity;

	struct d_aabb3 aabb = {
		{ 0 - half_dimension, 0 - half_dimension, 0 - half_dimension },
		{ half_dimension - 1, half_dimension - 1, half_dimension - 1 }
	};
	tree->root = d_octree_node_new (&aabb);
	tree->nnodes = 1;
	return tree;
}

void
d_octree_destroy (struct d_octree *tree) {
	if (tree) {
		d_octree_node_free (tree, tree->root);
		tree->root = 0;
		d_free (tree);
	}
}

struct d_octree_obj *
d_octree_insert (struct d_octree *tree, int x, int y, int z,
                 struct d_ob_instance *instance) {
	if (instance == 0) {
		return 0;
	}
	struct d_point3 pos = { x, y, z };
	if (!d_aabb3_contains (&tree->root->aabb, &pos)) {
		return 0;
	}

	struct d_octree_node *node = tree->root;
	for (;;) {
		if (node->children[0]) {
			for (int i=0;i<8;++i) {
				struct d_octree_node *cnode = node->children[i];
				if (d_aabb3_contains (&cnode->aabb, &pos)) {
					node = cnode;
					break;
				}
			}
		}
		else {
			struct d_point3 size;
			d_aabb3_size (&node->aabb, &size);
			if (node->objects->nnodes < tree->capacity || size.x == 2 || size.y == 2 || size.z == 2) {
				struct d_octree_obj *object = d_calloc (1, sizeof (struct d_octree_obj));
				object->pos.x = x;
				object->pos.y = y;
				object->pos.z = z;
				object->instance = instance;
				d_list_append (node->objects, object);
				tree->objects++;
				return object;
			}

			int hx = size.x / 2, hy = size.y / 2, hz = size.z / 2;
			if (hx % 2 != 0 || hy % 2 != 0 || hz % 2 != 0) {
				d_bug ("kaka %d, %d, %d", hx, hy, hz);
			}
			struct d_aabb3 aabb0 = { { node->aabb.p1.x, node->aabb.p1.y, node->aabb.p1.z }, { node->aabb.p1.x + hx - 1, node->aabb.p1.y + hy - 1, node->aabb.p1.z + hz - 1 } };
			struct d_aabb3 aabb1 = { { node->aabb.p1.x + hx, node->aabb.p1.y, node->aabb.p1.z }, { node->aabb.p2.x, node->aabb.p1.y + hy - 1, node->aabb.p1.z + hz - 1 } };
			struct d_aabb3 aabb2 = { { node->aabb.p1.x, node->aabb.p1.y + hy, node->aabb.p1.z }, { node->aabb.p1.x + hx - 1, node->aabb.p2.y, node->aabb.p1.z + hz - 1 } };
			struct d_aabb3 aabb3 = { { node->aabb.p1.x + hx, node->aabb.p1.y + hy, node->aabb.p1.z }, { node->aabb.p2.x, node->aabb.p2.y, node->aabb.p1.z + hz - 1 } };

			struct d_aabb3 aabb4 = { { node->aabb.p1.x, node->aabb.p1.y, node->aabb.p1.z + hz }, { node->aabb.p1.x + hx - 1, node->aabb.p1.y + hy - 1, node->aabb.p2.z } };
			struct d_aabb3 aabb5 = { { node->aabb.p1.x + hx, node->aabb.p1.y, node->aabb.p1.z + hz }, { node->aabb.p2.x, node->aabb.p1.y + hy - 1, node->aabb.p2.z } };
			struct d_aabb3 aabb6 = { { node->aabb.p1.x, node->aabb.p1.y + hy, node->aabb.p1.z + hz }, { node->aabb.p1.x + hx - 1, node->aabb.p2.y, node->aabb.p2.z } };
			struct d_aabb3 aabb7 = { { node->aabb.p1.x + hx, node->aabb.p1.y + hy, node->aabb.p1.z + hz }, { node->aabb.p2.x, node->aabb.p2.y, node->aabb.p2.z } };

			node->children[0] = d_octree_node_new (&aabb0);
			node->children[1] = d_octree_node_new (&aabb1);
			node->children[2] = d_octree_node_new (&aabb2);
			node->children[3] = d_octree_node_new (&aabb3);
			node->children[4] = d_octree_node_new (&aabb4);
			node->children[5] = d_octree_node_new (&aabb5);
			node->children[6] = d_octree_node_new (&aabb6);
			node->children[7] = d_octree_node_new (&aabb7);
			tree->nnodes += 8;
			for (struct d_list_node *obj_node = node->objects->first;obj_node;obj_node = obj_node->next) {
				struct d_octree_obj *obj = obj_node->data;
				for (int i=0;i<8;++i) {
					if (d_aabb3_contains (&node->children[i]->aabb, &obj->pos)) {
						d_list_append (node->children[i]->objects, obj);
					}
				}
			}
			d_list_clear (node->objects);
		}
	}
}

static int
d_octree_delete_cb (struct d_octree *tree, struct d_octree_node *node,
                    struct d_list *objects, struct d_list_node *obj_node,
                    struct d_octree_obj *object, void *data) {
	if (object == data) {
		d_octree_obj_free (object);
		d_list_remove (objects, obj_node);
		tree->objects--;
		return 0;
	}
	return 1;
}

void
d_octree_delete (struct d_octree *tree, struct d_octree_obj *object) {
	d_octree_traverse (tree, tree->root, d_octree_delete_cb, object);
}

void
d_octree_traverse (struct d_octree *tree, struct d_octree_node *node,
                   d_octree_cb cb, void *data) {
	d_octree_traverse_aabb (tree, node, &node->aabb, cb, data);
}

void
d_octree_traverse_aabb (struct d_octree *tree, struct d_octree_node *node,
                        struct d_aabb3 *aabb, d_octree_cb cb, void *data) {
	for (struct d_list_node *lnode=node->objects->first;lnode;lnode=lnode->next) {
		struct d_octree_obj *obj = lnode->data;
		if (d_aabb3_contains (aabb, &obj->pos)) {
			if (!cb (tree, node, node->objects, lnode, obj, data)) {
				return;
			}
		}
	}
	for (int i=0;i<8;++i) {
		/* TODO check if this aabb is intersecting with the query aabb */
		struct d_octree_node *child_node = node->children[i];
		if (child_node) {
			d_octree_traverse_aabb (tree, child_node, aabb, cb, data);
		}
	}
}

struct d_octree_traverse_result {
	struct d_aabb3 *aabb;
	struct d_octree_obj *match;
};

static int
d_octree_traverse_point_cb (struct d_octree *tree, struct d_octree_node *node,
                            struct d_list *objects, struct d_list_node *obj_node,
                            struct d_octree_obj *object, void *data) {
	struct d_octree_traverse_result *result = data;
	result->match = object;
	return 0;
}

struct d_octree_obj*
d_octree_traverse_point (struct d_octree *tree, struct d_point3 *point) {
	struct d_aabb3 aabb = { { point->x, point->y, point->z }, { point->x, point->y, point->z } };
	struct d_octree_traverse_result result = { &aabb, 0 };
	d_octree_traverse_aabb (tree, tree->root, &aabb, d_octree_traverse_point_cb, &result);
	return result.match;
}
