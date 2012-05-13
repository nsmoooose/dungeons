#include "object_tile.h"

static struct d_ob_instance* d_ob_create (struct d_ob_type *type, int x, int y, int z);
static void d_ob_destroy (struct d_ob_instance *inst);
static void d_ob_serialize (struct d_ob_instance *inst, struct d_storage *storage,
							enum d_ob_serialize_mode mode);

static struct d_ob_state d_ob_state_tile_nop = { "nop", 0, 0, 0 };

static struct d_ob_state_machine d_ob_sm_tile = {
	"tile",
	&d_ob_state_tile_nop,
	{ 0 },
	{ 0 }
};

static struct d_ob_type d_ob_type_tile = {
	"tile",
	"tile",
	&d_ob_sm_tile,
	0,
	{ 0 },
	d_ob_create,
	d_ob_destroy,
	d_ob_serialize
};

struct d_ob_category d_ob_category_tiles = {
	"tiles",
	{
		&d_ob_type_tile,
		0
	}
};

static struct d_ob_instance*
d_ob_create (struct d_ob_type *type, int x, int y, int z) {
	return 0;
}

static void d_ob_destroy (struct d_ob_instance *inst) {
}

static void d_ob_serialize (struct d_ob_instance *inst,
                            struct d_storage *storage,
                            enum d_ob_serialize_mode mode) {
}
