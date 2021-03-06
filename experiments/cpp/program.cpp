#include <check.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

void add_tree_tests (Suite *suite);

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
