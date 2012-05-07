#include <check.h>

#include "memory.h"
#include "world_gen.h"

START_TEST (test_world_free) {
	struct d_world_gen_params params;
	params.size = 8;

	struct d_world *world = d_world_generate (&params);
	ck_assert (world != 0);

	struct d_tile *tile = d_world_tile_get (world, 1, 1, 1, 1);
	ck_assert (tile != 0);

	d_world_free (world);
	ck_assert (d_allocations == d_frees);
}
END_TEST

void
add_world_tests (Suite *suite) {
	TCase *c = tcase_create ("world");
	tcase_add_test (c, test_world_free);
	suite_add_tcase (suite, c);
}

