#include <check.h>
#include "../memory.h"
#include "../octree.h"

START_TEST (test_octree_alloc) {
	struct d_octree *tree = d_octree_new (5, 256);
	d_octree_free (tree);

	ck_assert (d_octree_new (5, 10) == 0);
	ck_assert (d_octree_new (5, 9) == 0);

	tree = d_octree_new (5, 8);
	ck_assert (tree != 0);
	d_octree_free (tree);

	ck_assert (d_octree_new (5, 7) == 0);
	ck_assert (d_octree_new (5, 6) == 0);
	ck_assert (d_octree_new (5, 5) == 0);

	tree = d_octree_new (5, 4);
	ck_assert (tree != 0);
	d_octree_free (tree);

	ck_assert (d_octree_new (5, 3) == 0);

	tree = d_octree_new (5, 2);
	ck_assert (tree != 0);
	d_octree_free (tree);

	ck_assert (d_octree_new (5, 1) == 0);
	ck_assert (d_octree_new (5, 0) == 0);

	ck_assert (d_allocations == d_frees);
}
END_TEST

void
add_octree_tests (Suite *suite) {
	TCase *c = tcase_create ("octree");
	tcase_add_test (c, test_octree_alloc);
	suite_add_tcase (suite, c);
}
