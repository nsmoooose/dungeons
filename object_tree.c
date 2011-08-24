#include "object_tree.h"

struct d_ob_state d_ob_state_tree_growing = { "growing", 0, 0, 0 };
struct d_ob_state d_ob_state_tree_dead = { "dead", 0, 0, 0 };
struct d_ob_state d_ob_state_tree_on_fire = { "on fire", 0, 0, 0 };

struct d_ob_state_transition d_ob_transition_tree_growing_catch_fire = {
	"catch fire", &d_ob_state_tree_growing, &d_ob_state_tree_on_fire
};

struct d_ob_state_transition d_ob_transition_tree_dying_from_fire = {
	"dying", &d_ob_state_tree_on_fire, &d_ob_state_tree_dead
};

struct d_ob_state_transition d_ob_transition_tree_dying = {
	"dying", &d_ob_state_tree_growing, &d_ob_state_tree_dead
};

struct d_ob_state_transition d_ob_transition_tree_dead_catch_fire = {
	"catch fire", &d_ob_state_tree_dead, &d_ob_state_tree_on_fire
};

struct d_ob_state_machine d_ob_sm_tree = {
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

/* picea */
struct d_ob_type d_ob_type_tree_picea = {
	"picea",
	&d_ob_sm_tree,
	0, /* data */
	0, /* create */
	0, /* destroy */
	0 /* serialize */
};

struct d_ob_category d_ob_category_trees = {
	"trees",
	{
		&d_ob_type_tree_picea,
		0
	}
};
