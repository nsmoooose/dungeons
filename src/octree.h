#ifndef __DUNGEONS_OCTREE_H__
#define __DUNGEONS_OCTREE_H__

#include "math.h"
#include "object.h"

struct d_octree_obj {
	struct d_point3 pos;
	struct d_ob_instance *instance;
};

struct d_octree_node {
	struct d_aabb3 aabb;
	struct d_octree_node* children[8];
	struct d_list *objects;
};

struct d_octree {
	int capacity;
	struct d_octree_node *root;
	int nnodes;
	int objects;
};

typedef int (*d_octree_cb) (struct d_octree *tree, struct d_octree_node *node,
                            struct d_list *objects, struct d_list_node *obj_node,
                            struct d_octree_obj *object, void *data);

struct d_octree *d_octree_new (int capacity, int half_dimension);
void d_octree_destroy (struct d_octree *tree);
struct d_octree_obj *d_octree_insert (struct d_octree *tree, int x, int y, int z,
                                      struct d_ob_instance *instance);
void d_octree_delete (struct d_octree *tree, struct d_octree_obj *object);

void d_octree_traverse (struct d_octree *tree, struct d_octree_node *node,
                        d_octree_cb cb, void *data);
void d_octree_traverse_aabb (struct d_octree *tree, struct d_octree_node *node,
                             struct d_aabb3 *aabb, d_octree_cb cb, void *data);

#endif
