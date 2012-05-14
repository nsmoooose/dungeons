#include <math.h>
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
d_octree_node_new (int x, int y, int z, int half_dimension) {
	struct d_octree_node *node = d_calloc (1, sizeof (struct d_octree_node));
	node->aabb.pos.x = x;
	node->aabb.pos.y = y;
	node->aabb.pos.z = z;
	node->aabb.half_dimension = half_dimension;
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
	tree->root = d_octree_node_new (0, 0, 0, half_dimension);
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
	if (fabs (x) > tree->root->aabb.half_dimension ||
	    fabs (y) > tree->root->aabb.half_dimension ||
	    fabs (z) > tree->root->aabb.half_dimension) {
		return 0;
	}

	struct d_octree_node *node = tree->root;
	struct d_point3 pos = { x, y, z };
	for (;;) {
		if (node->children[0]) {
			for (int i=0;i<8;++i) {
				struct d_octree_node *cnode = node->children[i];
				if (d_aabb_contains (&cnode->aabb, &pos)) {
					node = cnode;
					break;
				}
			}
		}
		else {
			if (node->objects->nnodes < tree->capacity || node->aabb.half_dimension == 2) {
				struct d_octree_obj *object = d_calloc (1, sizeof (struct d_octree_obj));
				object->pos.x = x;
				object->pos.y = y;
				object->pos.z = z;
				object->instance = instance;
				d_list_append (node->objects, object);
				tree->objects++;
				return object;
			}
			else {
				int half = node->aabb.half_dimension / 2;
				node->children[0] = d_octree_node_new (node->aabb.pos.x - half, node->aabb.pos.y - half, node->aabb.pos.z - half, half);
				node->children[1] = d_octree_node_new (node->aabb.pos.x - half, node->aabb.pos.y - half, node->aabb.pos.z + half, half);
				node->children[2] = d_octree_node_new (node->aabb.pos.x - half, node->aabb.pos.y + half, node->aabb.pos.z - half, half);
				node->children[3] = d_octree_node_new (node->aabb.pos.x - half, node->aabb.pos.y + half, node->aabb.pos.z + half, half);
				node->children[4] = d_octree_node_new (node->aabb.pos.x + half, node->aabb.pos.y - half, node->aabb.pos.z - half, half);
				node->children[5] = d_octree_node_new (node->aabb.pos.x + half, node->aabb.pos.y - half, node->aabb.pos.z + half, half);
				node->children[6] = d_octree_node_new (node->aabb.pos.x + half, node->aabb.pos.y + half, node->aabb.pos.z - half, half);
				node->children[7] = d_octree_node_new (node->aabb.pos.x + half, node->aabb.pos.y + half, node->aabb.pos.z + half, half);
				tree->nnodes += 8;

				for (struct d_list_node *obj_node = node->objects->first;obj_node;obj_node = obj_node->next) {
					struct d_octree_obj *obj = obj_node->data;
					for (int i=0;i<8;++i) {
						if (d_aabb_contains (&node->children[i]->aabb, &obj->pos)) {
							d_list_append (node->children[i]->objects, obj);
						}
					}
				}
				d_list_clear (node->objects);
			}
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
	for (struct d_list_node *lnode=node->objects->first;lnode;lnode=lnode->next) {
		struct d_octree_obj *obj = lnode->data;
		if (!cb (tree, node, node->objects, lnode, obj, data)) {
			return;
		}
	}
	for (int i=0;i<8;++i) {
		struct d_octree_node *child_node = node->children[i];
		if (child_node) {
			d_octree_traverse (tree, child_node, cb, data);
		}
	}
}

void
d_octree_traverse_aabb (struct d_octree *tree, struct d_octree_node *node, int x1,
                        int y1, int x2, int y2, d_octree_cb cb, void *data) {
}
