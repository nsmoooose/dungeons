#include <check.h>
#include "../math.h"

START_TEST (test_aabb) {
	struct d_aabb aabb = { { 0, 0, 0 }, 4 };

	struct d_point3 pos1 = { 0, 0, 0 };
	ck_assert (d_aabb_contains (&aabb, &pos1) == 1);
	struct d_point3 pos2 = { -4, -4, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos2) == 1);

	struct d_point3 pos3 = { -4, -4, -5 };
	ck_assert (d_aabb_contains (&aabb, &pos3) == 0);
	struct d_point3 pos4 = { -4, -5, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos4) == 0);
	struct d_point3 pos5 = { -5, -4, -4 };
	ck_assert (d_aabb_contains (&aabb, &pos5) == 0);

	struct d_point3 pos6 = { 4, 3, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos6) == 0);
	struct d_point3 pos7 = { 3, 4, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos7) == 0);
	struct d_point3 pos8 = { 3, 3, 4 };
	ck_assert (d_aabb_contains (&aabb, &pos8) == 0);

	struct d_point3 pos9 = { 3, 3, 3 };
	ck_assert (d_aabb_contains (&aabb, &pos9) == 1);
}
END_TEST

void
add_math_tests (Suite *suite) {
	TCase *c = tcase_create ("math");
	tcase_add_test (c, test_aabb);
	suite_add_tcase (suite, c);
}
