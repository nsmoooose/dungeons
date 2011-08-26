#include <stdio.h>

#include "game.h"
#include "memory.h"
#include "object.h"

struct d_game_context*
d_game_context_new () {
	struct d_game_context *context = d_calloc (1, sizeof (struct d_game_context));
	context->objects = d_ob_list_new ();
	return context;
}

void
d_game_context_destroy (struct d_game_context *context) {
	d_free (context);
}

void
d_game_run (struct d_game_context *context, double now, double delta) {
	/* A day is 86400 seconds. We want a day to pass every 10 seconds. */
	double second = 86400 / 10.0;
	context->delta = delta * second;

	for (struct d_list_node *node = context->objects->first;node;node=node->next) {
		struct d_ob_instance *inst = node->data;
		inst->state->input (context, inst);
	}

	context->datetime += context->delta;
}

void
d_game_format_date (char *buffer, int buffer_size, double datetime) {
	int days = datetime / (60 * 60 * 24);

	int year = days / 372;
	int month = (days % 372) / 31;
	int day = (days % 372) % 31;

	snprintf (buffer, buffer_size, "%03d-%02d-%02d", year, month, day);
}
