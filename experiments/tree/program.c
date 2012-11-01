#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tree_type_data {
	/* growth rage in cm per year */
	char growth_rage;
};

struct tree {
	struct tree_type_data *type;
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
	t->age += d;
}

START_TEST (test_tree_update) {
	struct tree_type_data type = { 20 };
	struct tree t = { &type, 0, 0 };

	tree_update (&t, 4);
	ck_assert (t.age == 4);
	tree_update (&t, 3);
	ck_assert (t.age == 7);
}
END_TEST

void add_tree_tests (Suite *suite) {
	TCase *c = tcase_create ("tree");
	tcase_add_test (c, test_tree_update);
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





	struct tree t;

	double now = d_time_get ();
	double last = now;
	for (;;) {
		now = d_time_get ();
		double delta = now - last;

		tree_update (&t, delta);

		last = now;
	}
}
