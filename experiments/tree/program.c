#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct pos {
	double x,y,z;
};

enum living {
	alive,
	dead
};

struct tree_type_data {
	/* growth rage in cm per year */
	unsigned short growth_rate;
	short max_height;
	short min_height;
};

struct tree {
	struct tree_type_data *type;
	struct pos *pos;
	enum living state;
	double age;
	double height;
};

static double d_time_get () {
	struct timespec clock;
	if (clock_gettime (CLOCK_REALTIME, &clock) == -1) {
		/* d_bug ("Failed to get clock."); */
	}
	return clock.tv_sec + ((double)clock.tv_nsec / 1000000000.0);
}

static void tree_update (struct tree* t, double d) {
	/* TODO tree should slowly die outside its normal habitat.
	/* Tree should die outside of its normal habitat.
	 * It would be better to slowly die than to just die quickly. But
	 * this can be an improvement for the future. */
	if (t->pos->z > t->type->max_height || t->pos->z < t->type->min_height) {
		t->state = dead;
		return;
	}
	t->age += d;
	t->height += (t->type->growth_rate / 365.0 * d);
}

START_TEST (test_tree_growth_rate) {
	struct tree_type_data type = { 365, 2000, -2000 };
	struct pos p = { 0, 0, 0 };
	struct tree t = { &type, &p, alive, 0, 0 };

	/* growth rate testing */
	tree_update (&t, 4);
	ck_assert (t.age == 4);
	ck_assert (t.height == 4);
	tree_update (&t, 3);
	ck_assert (t.age == 7);
	ck_assert (t.height == 7);

	/* growth rate with a different rate. */
	type.growth_rate = 10;
	t.height = 0;
	tree_update (&t, 365);
	ck_assert (t.height == 10);
	ck_assert (t.state == alive);

}
END_TEST

START_TEST (test_tree_outside_habitat) {
	struct tree_type_data type = { 365, 2000, -2000 };
	struct pos p = { 0, 0, 0 };
	struct tree t = { &type, &p, alive, 0, 0 };

	/* die outside its habitat */
	t.pos->z = 2001;
	tree_update (&t, 365);
	ck_assert (t.state == dead);
	t.state = alive;
	t.pos->z = -2001;
	tree_update (&t, 365);
	ck_assert (t.state == dead);
}
END_TEST

void add_tree_tests (Suite *suite) {
	TCase *c = tcase_create ("tree");
	tcase_add_test (c, test_tree_growth_rate);
	tcase_add_test (c, test_tree_outside_habitat);
	suite_add_tcase (suite, c);
}

int main (int argc, char *argv[]) {
	int failed;
	SRunner* runner;
	Suite* suite = suite_create("dungeons");
	add_tree_tests (suite);

	runner = srunner_create (suite);
	if (argc == 2 && strcmp (argv[1], "--debug") == 0) {
		srunner_set_fork_status (runner, CK_NOFORK);
	}
	srunner_run_all (runner, CK_VERBOSE);
	srunner_print (runner, CK_MINIMAL);

	failed = srunner_ntests_failed (runner);
	srunner_free (runner);

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
