#include <check.h>
#include "../memory.h"
#include "../object.h"

START_TEST (test_prop_pos3) {
	struct d_prop_instance *inst = d_prop_pos3.create (&d_prop_pos3);
	ck_assert (inst != 0);
	ck_assert (inst->type == &d_prop_pos3);
	ck_assert (inst->value != 0);
	struct d_point3 *pos = d_prop_pos3_get (inst);
	ck_assert (pos != 0);
	inst->type->destroy (inst);
	ck_assert (d_allocations == d_frees);
}
END_TEST

START_TEST (test_prop_string) {
	struct d_prop_instance *inst = d_prop_str.create (&d_prop_str);
	ck_assert (inst != 0);
	ck_assert (inst->type == &d_prop_str);
	ck_assert (inst->value == 0);
	d_prop_str_set (inst, "kaka");
	ck_assert (strcmp ("kaka", d_prop_str_get (inst)) == 0);
	inst->type->destroy (inst);
	ck_assert (d_allocations == d_frees);
}
END_TEST

START_TEST (test_prop_float) {
	struct d_prop_instance *inst = d_prop_float.create (&d_prop_float);
	ck_assert (inst != 0);
	ck_assert (inst->type == &d_prop_float);
	ck_assert (inst->value != 0);
	ck_assert (d_prop_float_get (inst) == 0);
	d_prop_float_set (inst, 5);
	ck_assert (d_prop_float_get (inst) == 5);
	inst->type->destroy (inst);
	ck_assert (d_allocations == d_frees);
}
END_TEST

START_TEST (test_prop_int) {
	struct d_prop_instance *inst = d_prop_int.create (&d_prop_int);
	ck_assert (inst != 0);
	ck_assert (inst->type == &d_prop_int);
	ck_assert (inst->value != 0);
	ck_assert (d_prop_int_get (inst) == 0);
	d_prop_int_set (inst, 5);
	ck_assert (d_prop_int_get (inst) == 5);
	inst->type->destroy (inst);
	ck_assert (d_allocations == d_frees);
}
END_TEST

void
add_object_tests (Suite *suite) {
	TCase *c = tcase_create ("object");
	tcase_add_test (c, test_prop_pos3);
	tcase_add_test (c, test_prop_string);
	tcase_add_test (c, test_prop_float);
	tcase_add_test (c, test_prop_int);
	suite_add_tcase (suite, c);
}
