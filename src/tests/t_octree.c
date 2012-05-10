#include <check.h>
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
	ck_assert (tree->nnodes == 0);

	obj = d_octree_insert (tree, 50, 50, 50, instance);
	ck_assert (obj != 0);
	ck_assert (tree->nnodes == 1);

	d_octree_delete (tree, obj);
	ck_assert (tree->nnodes == 0);
	d_octree_destroy (tree);

	tree = d_octree_new (3, 256);
	struct d_octree_obj *obj1 = d_octree_insert (tree, 50, 50, 50, instance);
	struct d_octree_obj *obj2 = d_octree_insert (tree, -50, -50, -50, instance);
	struct d_octree_obj *obj3 = d_octree_insert (tree, 50, 50, -50, instance);
	ck_assert (obj1 != 0 && obj2 != 0 && obj3 != 0);
	ck_assert (tree->nnodes == 3);
	ck_assert (tree->root->objects->nnodes == 3);

	struct d_octree_obj *obj4 = d_octree_insert (tree, 50, -50, 50, instance);
	ck_assert (obj4 != 0);
	ck_assert (tree->nnodes == 4);
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

	d_octree_destroy (tree);
	type->destroy (instance);
	ck_assert_msg (d_allocations == d_frees, "Allocations: %d, Frees: %d", d_allocations, d_frees);
}
END_TEST

START_TEST (test_aabb) {
	struct d_aabb aabb = { { 0, 0, 0 }, 4 };

	struct d_ob_pos pos1 = { 0, 0, 0 };
	ck_assert (d_aabb_contains (&aabb, &pos1) == 1);
	struct d_ob_pos pos2 = { -4, -4, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos2) == 1);

	struct d_ob_pos pos3 = { -4, -4, -5 };
	ck_assert (d_aabb_contains (&aabb, &pos3) == 0);
	struct d_ob_pos pos4 = { -4, -5, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos4) == 0);
	struct d_ob_pos pos5 = { -5, -4, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos5) == 0);

	struct d_ob_pos pos6 = { 4, 3, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos6) == 0);
	struct d_ob_pos pos7 = { 3, 4, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos7) == 0);
	struct d_ob_pos pos8 = { 3, 3, 4 };
	ck_assert (d_aabb_contains (&aabb, &pos8) == 0);

	struct d_ob_pos pos9 = { 3, 3, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos9) == 1);
}
END_TEST

void
add_octree_tests (Suite *suite) {
	TCase *c = tcase_create ("octree");
	tcase_add_test (c, test_octree_alloc);
	tcase_add_test (c, test_octree_insert_delete);
	tcase_add_test (c, test_aabb);
	suite_add_tcase (suite, c);
}
