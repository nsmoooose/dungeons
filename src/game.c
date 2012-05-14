#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "error.h"
#include "game.h"
#include "memory.h"
#include "object.h"
#include "storage.h"
#include "str.h"

struct d_game_context*
d_game_context_new () {
	struct d_game_context *context = d_calloc (1, sizeof (struct d_game_context));
	context->vp = d_calloc (1, sizeof (struct d_viewpoint));
	context->objects = d_ob_list_new ();
	return context;
}

void
d_game_context_destroy (struct d_game_context *context) {
	d_free (context->directory);
	d_free (context->vp);
	d_list_destroy (context->objects);
	d_free (context);
}

void
d_game_run (struct d_game_context *context, double now, double delta) {
	/* A day is 86400 seconds. We want a day to pass every 10 seconds. */
	double second = 86400 / 10.0;
	context->delta = delta * second;

	for (struct d_list_node *node = context->objects->first;node;node=node->next) {
		struct d_ob_instance *inst = node->data;
		inst->state->input (inst);
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

static int
d_dir_exists (const char *path) {
	DIR *d = opendir (path);
	if (d == 0) {
		return 0;
	}
	closedir (d);
	return 1;
}

/*
static int
d_file_exists (const char *path) {
	struct stat st;
	return stat (path, &st) == -1 ? 0 : 1;
}
*/

static void
d_game_info_list_delete (void *data) {
	struct d_game_info *info = data;
	d_strfree (info->description);
	d_strfree (info->path);
	d_free (info);
}

struct d_list *
d_game_info_list () {
	struct d_list *list = d_list_new ();
	list->remove = d_game_info_list_delete;

	/* TODO Set a item destroy function to the list of game info */

	char *home = getenv ("HOME");
	char dungeons_path[1024];
	snprintf (dungeons_path, 1024, "%s/.dungeons", home);

	if (!d_dir_exists (dungeons_path)) {
		if (mkdir (dungeons_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
			d_bug ("Failed to create ~/.dungeons");
		}
	}

	char saves_path[1024];
	snprintf (saves_path, 1024, "%s/saves", dungeons_path);
	if (!d_dir_exists (saves_path)) {
		if (mkdir (saves_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
			d_bug ("Failed to create ~/.dungeons/saves");
		}
	}

	DIR *d = opendir (saves_path);
	if (!d) {
		d_bug ("Failed to open ~/.dungeons/saves directory.");
	}
	struct dirent *dir = 0;
	while ((dir=readdir (d))) {
		if (strcmp (".", dir->d_name) == 0 ||
			strcmp ("..", dir->d_name) == 0) {
			continue;
		}

		struct d_game_info *info = d_calloc (1, sizeof (struct d_game_info));
		char buffer[1024];
		snprintf (buffer, 1024, "%s/%s", saves_path, dir->d_name);
		info->path = d_strdup (buffer);
		info->description = d_strdup (dir->d_name);
		d_list_append (list, info);
	}
	closedir (d);

	/* TODO sort game info list? */

	return list;
}

void
d_game_save (struct d_game_context *context) {
	struct d_storage *storage = d_storage_new (context->directory, "time");
	d_storage_write_d (storage, &context->datetime);
	d_storage_close (storage);

	storage = d_storage_new (context->directory, "hm");
	d_storage_write_i (storage, &context->hm->width);
	for (int x=0;x<=context->hm->width;++x) {
		for (int y=0;y<=context->hm->height;++y) {
			int h = d_heightmap_get (context->hm, x, y);
			d_storage_write_i (storage, &h);
		}
	}
	d_storage_close (storage);

	storage = d_storage_new (context->directory, "viewpoint");
	d_storage_write_i (storage, &context->vp->x);
	d_storage_write_i (storage, &context->vp->y);
	d_storage_write_i (storage, &context->vp->z);
	d_storage_write_i (storage, &context->zoom_level);
	d_storage_close (storage);

	storage = d_storage_new (context->directory, "objects");
	d_storage_write_i (storage, &context->objects->nnodes);
	struct d_list_node *node = context->objects->first;
	for (;node;node=node->next) {
		struct d_ob_instance *instance = node->data;
		struct d_ob_type *type = instance->type;
		d_storage_write_s (storage, type->id);
		d_storage_write_s (storage, instance->state->id);
		d_storage_write_i (storage, &instance->pos.x);
		d_storage_write_i (storage, &instance->pos.y);
		d_storage_write_i (storage, &instance->pos.z);
		type->serialize (instance, storage, d_ob_write);
	}
	d_storage_close (storage);
}

struct d_game_context *
d_game_load (char *directory) {
	struct d_game_context *context = d_game_context_new ();

	context->directory = d_strdup (directory);

	struct d_storage* storage = d_storage_new (directory, "time");
	d_storage_read_d (storage, &context->datetime);
	d_storage_close (storage);

	storage = d_storage_new (directory, "hm");
	int size;
	d_storage_read_i (storage, &size);
	context->hm = d_heightmap_new (size);
	for (int x=0;x<=context->hm->width;++x) {
		for (int y=0;y<=context->hm->height;++y) {
			int h;
			d_storage_read_i (storage, &h);
			d_heightmap_set (context->hm, x, y, h);
		}
	}
	d_storage_close (storage);

	storage = d_storage_new (directory, "viewpoint");
	d_storage_read_i (storage, &context->vp->x);
	d_storage_read_i (storage, &context->vp->y);
	d_storage_read_i (storage, &context->vp->z);
	d_storage_read_i (storage, &context->zoom_level);
	d_storage_close (storage);

	storage = d_storage_new (directory, "objects");
	int object_count;
	d_storage_read_i (storage, &object_count);
	for (int i=0;i<object_count;++i) {
		char *type_id = d_storage_read_s (storage);
		char *state_id = d_storage_read_s (storage);
		int x, y, z;
		d_storage_read_i (storage, &x);
		d_storage_read_i (storage, &y);
		d_storage_read_i (storage, &z);

		struct d_ob_type *type = d_ob_get_type (&d_ob_registry, type_id);
		struct d_ob_state *state = d_ob_get_state (type->sm, state_id);
		struct d_ob_instance *instance = type->create (type, x, y, z);
		instance->state = state;
		type->serialize (instance, storage, d_ob_read);

		d_free (type_id);
		d_free (state_id);

		d_list_append (context->objects, instance);
	}
	d_storage_close (storage);

	return context;
}
