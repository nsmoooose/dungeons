#include <check.h>
#include "../math.h"

START_TEST (test_aabb3) {
	struct d_aabb3 aabb = { { -4, -4, -4 }, { 4, 4, 4 } };

	struct d_point3 pos1 = { 0, 0, 0 };
	ck_assert (d_aabb3_contains (&aabb, &pos1) == 1);
	struct d_point3 pos2 = { -4, -4, -4 };
	ck_assert (d_aabb3_contains (&aabb, &pos2) == 1);

	struct d_point3 pos3 = { -4, -4, -5 };
	ck_assert (d_aabb3_contains (&aabb, &pos3) == 0);
	struct d_point3 pos4 = { -4, -5, -4 };
	ck_assert (d_aabb3_contains (&aabb, &pos4) == 0);
	struct d_point3 pos5 = { -5, -4, -4 };
	ck_assert (d_aabb3_contains (&aabb, &pos5) == 0);

	struct d_point3 pos6 = { 5, 3, 3 };
	ck_assert (d_aabb3_contains (&aabb, &pos6) == 0);
	struct d_point3 pos7 = { 3, 5, 3 };
	ck_assert (d_aabb3_contains (&aabb, &pos7) == 0);
	struct d_point3 pos8 = { 3, 3, 5 };
	ck_assert (d_aabb3_contains (&aabb, &pos8) == 0);

	struct d_point3 pos9 = { 4, 4, 4 };
	ck_assert (d_aabb3_contains (&aabb, &pos9) == 1);
}
END_TEST

START_TEST (test_aabb3_size) {
	struct d_point3 pos;

	struct d_aabb3 aabb1 = { { -1, -2, -3 }, { 1, 2, 3 } };
	d_aabb3_size (&aabb1, &pos);
	ck_assert (pos.x == 3);
	ck_assert (pos.y == 5);
	ck_assert (pos.z == 7);

	struct d_aabb3 aabb2 = { { 1, 2, 3 }, { -1, -2, -3 } };
	d_aabb3_size (&aabb2, &pos);
	ck_assert (pos.x == 3);
	ck_assert (pos.y == 5);
	ck_assert (pos.z == 7);
}
END_TEST

void
add_math_tests (Suite *suite) {
	TCase *c = tcase_create ("math");
	tcase_add_test (c, test_aabb3);
	tcase_add_test (c, test_aabb3_size);
	suite_add_tcase (suite, c);
}
