#ifndef __DUNGEONS_GAME_H__
#define __DUNGEONS_GAME_H__

#include "list.h"

struct d_game_context {
	/* Current date and time. */
	double datetime;
	/* Delta since the last update. */
	double delta;

	struct d_list *objects;
};

struct d_game_context* d_game_context_new ();
void d_game_context_destroy (struct d_game_context *context);

void d_game_run (struct d_game_context *context, double now, double delta);

void d_game_format_date (char *buffer, int buffer_size, double date);

void d_game_save (struct d_game_context *context, char *directory);
void d_game_load (struct d_game_context *context, char *directory);

#endif
