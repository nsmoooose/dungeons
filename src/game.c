#include <stdio.h>

#include "game.h"
#include "memory.h"
#include "object.h"
#include "storage.h"

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

struct d_list *
d_game_info_list () {
	struct d_list *list = d_list_new ();
	

	return list;
}

void
d_game_save (struct d_game_context *context) {
	struct d_storage *storage = d_storage_new (context->directory, "time");
	d_storage_write_d (storage, "datetime", &context->datetime);
	d_storage_close (storage);
}

struct d_game_context *
d_game_load (char *directory) {
	struct d_game_context *context = d_game_context_new ();
	struct d_storage* storage = d_storage_new (directory, "time");
	d_storage_read_d (storage, "datetime", &context->datetime);
	d_storage_close (storage);
	return context;
}
