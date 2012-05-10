#include <check.h>
#include "../math.h"
#include "../memory.h"
#include "../object_tree.h"
#include "../octree.h"

START_TEST (test_octree_alloc) {
	struct d_octree *tree = d_octree_new (5, 256);
	d_octree_destroy (tree);

	ck_assert (d_octree_new (5, 10) == 0);
	ck_assert (d_octree_new (5, 9) == 0);

	tree = d_octree_new (5, 8);
	ck_assert (tree != 0);
	d_octree_destroy (tree);

	ck_assert (d_octree_new (5, 7) == 0);
	ck_assert (d_octree_new (5, 6) == 0);
	ck_assert (d_octree_new (5, 5) == 0);

	tree = d_octree_new (5, 4);
	ck_assert (tree != 0);
	d_octree_destroy (tree);

	ck_assert (d_octree_new (5, 3) == 0);

	tree = d_octree_new (5, 2);
	ck_assert (tree != 0);
	d_octree_destroy (tree);

	ck_assert (d_octree_new (5, 1) == 0);
	ck_assert (d_octree_new (5, 0) == 0);

	ck_assert (d_allocations == d_frees);
}
END_TEST

START_TEST (test_octree_insert_delete) {
	struct d_octree *tree = d_octree_new (3, 256);

	struct d_ob_type *type = d_ob_category_trees.objects[0];
	struct d_ob_instance *instance = type->create (type, 1, 2, 3);

	struct d_octree_obj *obj = d_octree_insert (tree, 5000, 5000, 5000, instance);
	ck_assert (obj == 0);
	ck_assert (tree->objects == 0);

	obj = d_octree_insert (tree, 50, 51, 52, instance);
	ck_assert (obj != 0);
	ck_assert (obj->pos.x == 50);
	ck_assert (obj->pos.y == 51);
	ck_assert (obj->pos.z == 52);
	ck_assert (obj->instance == instance);
	ck_assert (tree->objects == 1);

	d_octree_delete (tree, obj);
	ck_assert (tree->objects == 0);
	d_octree_destroy (tree);

	tree = d_octree_new (3, 256);
	struct d_octree_obj *obj1 = d_octree_insert (tree, 50, 50, 50, instance);
	struct d_octree_obj *obj2 = d_octree_insert (tree, -50, -50, -50, instance);
	struct d_octree_obj *obj3 = d_octree_insert (tree, 50, 50, -50, instance);
	ck_assert (obj1 != 0 && obj2 != 0 && obj3 != 0);
	ck_assert (tree->objects == 3);
	ck_assert (tree->root->objects->nnodes == 3);

	struct d_octree_obj *obj4 = d_octree_insert (tree, 50, -50, 50, instance);
	ck_assert (obj4 != 0);
	ck_assert (tree->objects == 4);
	ck_assert (tree->root->children[0] != 0);
	ck_assert (tree->root->children[1] != 0);
	ck_assert (tree->root->children[2] != 0);
	ck_assert (tree->root->children[3] != 0);
	ck_assert (tree->root->children[4] != 0);
	ck_assert (tree->root->children[5] != 0);
	ck_assert (tree->root->children[6] != 0);
	ck_assert (tree->root->children[7] != 0);
	ck_assert_msg (tree->root->objects->nnodes == 0,
	               "nnodes was: %d", tree->root->objects->nnodes);
	ck_assert (tree->nnodes == 9);

	d_octree_destroy (tree);
	type->destroy (instance);
	ck_assert_msg (d_allocations == d_frees, "Allocations: %d, Frees: %d", d_allocations, d_frees);
}
END_TEST

static int
test_octree_object_delete_cb (struct d_octree *tree, struct d_octree_node *node,
                              struct d_list *objects, struct d_list_node *obj_node,
                              struct d_octree_obj *object, void *data) {
	object->instance->type->destroy (object->instance);
	return 1;
}

START_TEST (test_octree_10k_objects) {
	struct d_octree *tree = d_octree_new (10, 256);
	struct d_ob_type *type = d_ob_category_trees.objects[0];

	for (int i=0;i<10000;++i) {
		struct d_ob_instance *instance = type->create (type, 1, 2, 3);
		ck_assert (instance != 0);
		int x = d_rand (-100, 100);
		int y = d_rand (-100, 100);
		int z = d_rand (-100, 100);
		struct d_octree_obj *object = d_octree_insert (tree, x, y, z, instance);
		ck_assert (object != 0);
	}

	printf ("Nodes %d\n", tree->nnodes);

	ck_assert (tree->objects == 10000);
	d_octree_traverse (tree, tree->root, test_octree_object_delete_cb, 0);

	d_octree_destroy (tree);
	ck_assert_msg (d_allocations == d_frees, "Allocations: %d, Frees: %d", d_allocations, d_frees);
}
END_TEST

void
add_octree_tests (Suite *suite) {
	TCase *c = tcase_create ("octree");
	tcase_add_test (c, test_octree_alloc);
	tcase_add_test (c, test_octree_insert_delete);
	tcase_add_test (c, test_octree_10k_objects);
	suite_add_tcase (suite, c);
}
