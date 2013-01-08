#include <check.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "o_tree.hpp"

START_TEST (test_tree_growth_rate) {
	struct pos p = { 0, 0, 0 };
	tree_type *type = new tree_type ();
	type->growth_rate = 365;
	type->max_height = 2000;
	type->min_height = -2000;

	tree_object *tree = dynamic_cast<tree_object*>(type->create_instance ());
	tree->pos = &p;

	ck_assert (tree != 0);

	/* growth rate testing */
	tree->update (4);
	ck_assert (tree->age == 4);
	ck_assert (tree->height == 4);
	tree->update (3);
	ck_assert (tree->age == 7);
	ck_assert (tree->height == 7);

	/* growth rate with a different rate. */
	type->growth_rate = 10;
	tree->height = 0;
	tree->update (365);
	ck_assert (tree->height == 10);
	ck_assert (tree->state == alive);
}
END_TEST

START_TEST (test_tree_outside_habitat) {
	struct pos p = { 0, 0, 0 };
	tree_type *type = new tree_type ();
	type->growth_rate = 365;
	type->max_height = 2000;
	type->min_height = -2000;

	tree_object *tree = dynamic_cast<tree_object*>(type->create_instance ());
	tree->pos = &p;

	/* die outside its habitat */
	tree->pos->z = 2001;
	tree->update (365);
	ck_assert (tree->state == dead);
	tree->state = alive;
	tree->pos->z = -2001;
	tree->update (365);
	ck_assert (tree->state == dead);
}
END_TEST

void add_tree_tests (Suite *suite) {
	TCase *c = tcase_create ("tree");
	tcase_add_test (c, test_tree_growth_rate);
	tcase_add_test (c, test_tree_outside_habitat);
	suite_add_tcase (suite, c);
}
