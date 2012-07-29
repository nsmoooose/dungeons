#ifndef __DUNGEONS_GAME_H__
#define __DUNGEONS_GAME_H__

#include "heightmap.h"
#include "list.h"

struct d_viewpoint {
	int x, y, z;
};

struct d_game_context {
	char *directory;

	/* Current date and time. */
	double datetime;
	/* Delta since the last update. */
	double delta;

	struct d_heightmap *hm;
	struct d_viewpoint *vp;
	int zoom_level;
	struct d_list *objects;
};

struct d_game_context* d_game_context_new ();
void d_game_context_destroy (struct d_game_context *context);

void d_game_run (struct d_game_context *context, double now, double delta);

void d_game_format_date (char *buffer, int buffer_size, double date);

int d_game_time_days (double datetime);

struct d_game_info {
	char *path;
	char *description;
};

struct d_list *d_game_info_list ();

void d_game_save (struct d_game_context *context);
struct d_game_context * d_game_load (char *directory);

#endif
