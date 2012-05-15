#include <check.h>
#include "../object.h"

START_TEST (test_object_a) {
}
END_TEST

void
add_object_tests (Suite *suite) {
	TCase *c = tcase_create ("object");
	tcase_add_test (c, test_object_a);
	suite_add_tcase (suite, c);
}
