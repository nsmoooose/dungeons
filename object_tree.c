#include <string.h>

#include "error.h"
#include "memory.h"
#include "object_tree.h"

static struct d_ob_instance* d_ob_create (struct d_ob_type *type, struct d_ob_pos *pos);
static void d_ob_destroy (struct d_ob_instance *inst);
static void d_ob_serialize (struct d_ob_instance *inst);
static void d_ob_growing_input (struct d_game_context *c, struct d_ob_instance *inst);
static void d_ob_dead_input (struct d_game_context *c, struct d_ob_instance *inst);
static void d_ob_on_fire_input (struct d_game_context *c, struct d_ob_instance *inst);

static struct d_ob_state d_ob_state_tree_growing = { "growing", 0, d_ob_growing_input, 0 };
static struct d_ob_state d_ob_state_tree_dead = { "dead", 0, d_ob_dead_input, 0 };
static struct d_ob_state d_ob_state_tree_on_fire = { "on fire", 0, d_ob_on_fire_input, 0 };

static struct d_ob_state_transition d_ob_transition_tree_growing_catch_fire = {
	"catch fire", &d_ob_state_tree_growing, &d_ob_state_tree_on_fire
};

static struct d_ob_state_transition d_ob_transition_tree_dying_from_fire = {
	"dying", &d_ob_state_tree_on_fire, &d_ob_state_tree_dead
};

static struct d_ob_state_transition d_ob_transition_tree_dying = {
	"dying", &d_ob_state_tree_growing, &d_ob_state_tree_dead
};

static struct d_ob_state_transition d_ob_transition_tree_dead_catch_fire = {
	"catch fire", &d_ob_state_tree_dead, &d_ob_state_tree_on_fire
};

static struct d_ob_state_machine d_ob_sm_tree = {
	"tree",
	&d_ob_state_tree_growing,
	{
		&d_ob_state_tree_dead,
		&d_ob_state_tree_growing,
		&d_ob_state_tree_on_fire,
		0
	},
	{
		&d_ob_transition_tree_dead_catch_fire,
		&d_ob_transition_tree_dying,
		&d_ob_transition_tree_dying_from_fire,
		&d_ob_transition_tree_growing_catch_fire,
		0
	}
};

struct d_tree_type_data {
	int max_age;
	int max_height;

	int growing_zone_min;
	int growing_zone_max;

	int growing_period_min;
	int growing_period_max;
	int growing_temp_min;
	int growing_temp_max;

	int seeding_period_min;
	int seeding_period_max;
};

struct d_tree_inst_data {
	int age;
	int height;
};

static struct d_ob_type d_ob_type_tree_picea = {
	"picea",
	&d_ob_sm_tree,
	0, /* data */
	d_ob_create,
	d_ob_destroy,
	d_ob_serialize
};

struct d_ob_category d_ob_category_trees = {
	"trees",
	{
		&d_ob_type_tree_picea,
		0
	}
};

static struct d_ob_instance*
d_ob_create (struct d_ob_type *type, struct d_ob_pos *pos) {
	struct d_ob_instance *inst = d_calloc (1, sizeof (struct d_ob_instance));
	inst->type = type;
	inst->state = type->sm->start;
	inst->data = d_calloc (1, sizeof (struct d_tree_inst_data));
	memcpy (&inst->pos, pos, sizeof (struct d_ob_pos));
	return inst;
}

static void
d_ob_destroy (struct d_ob_instance *inst) {
	d_free (inst->data);
	d_free (inst);
}

static void
d_ob_serialize (struct d_ob_instance *inst) {
	d_bug ("NOT IMPLEMENTED");
}

static void
d_ob_growing_input (struct d_game_context *context, struct d_ob_instance *inst) {
	struct d_tree_type_data *td = inst->type->data;
	/* struct d_tree_inst_data *id = inst->data; */
	if (inst->pos.z >= td->growing_zone_max || inst->pos.z <= td->growing_zone_min) {
		/* TODO transition to dead. */
		return;
	}
}

static void
d_ob_dead_input (struct d_game_context *context, struct d_ob_instance *inst) {
	struct d_tree_inst_data *id = inst->data;
	if (id->height == 0) {
		/* A seed that never took root. */
		/* TODO schedule a removal. */
		return;
	}
	/* TODO some kind of decaying until the tree is completely gone or useless
	   to us. */

	/* TODO alert that this object should be removed. */

	/* TODO a damage level will increase with the decaying time of this object. */
}

static void
d_ob_on_fire_input (struct d_game_context *context, struct d_ob_instance *inst) {
	/* TODO fire will last until the entire tree is consumed */

	/* TODO fire is put out with water (raining) and we move to the dead state. */
}
