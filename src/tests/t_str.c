#include <check.h>

#include "../memory.h"
#include "../str.h"

START_TEST (test_strdup) {
	char *s = d_strdup ("kaka");
	ck_assert (strcmp (s, "kaka") == 0);
	d_strfree (s);
	ck_assert (1 == d_allocations);
}
END_TEST

void
add_str_tests (Suite *suite) {
	TCase *c = tcase_create ("str");
	tcase_add_test (c, test_strdup);
	suite_add_tcase (suite, c);
}
