#include <curses.h>
#include <libintl.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#include "../clock.h"
#include "../error.h"
#include "../storage.h"
#include "esc_menu.h"
#include "gamescreen.h"
#include "load_game.h"
#include "main_menu.h"
#include "new_world.h"
#include "quit.h"
#include "ui.h"

struct d_ui *d_ui = 0;
extern struct d_ui_state_machine d_ui_state_machine;

void
d_ui_do_transition (struct d_ui_state_transition *transition) {
	struct d_ui_state *current = d_ui_state_current ();
	for (int i=0;d_ui_state_machine.transitions[i];++i) {
		if (transition == d_ui_state_machine.transitions[i]) {
			if (transition->from == current) {
				if (current->exit) {
					current->exit (current, transition->next);
				}
				if (transition->next->enter) {
					transition->next->enter (current, transition->next);
				}
				d_ui_state_machine.current = transition->next;
				return;
			}
			else {
				d_bug ("Cannot perform transition: \"%s\"", transition->description);
			}
		}
	}
	d_bug ("Transition not found in state machine. \"%s\"", transition->description);
}

struct d_ui_state*
d_ui_state_current () {
	return d_ui_state_machine.current;
}

void
d_ui_state_machine_print (char *filename) {
	FILE *f = fopen (filename, "w");
	if (!f) {
		d_bug ("Failed to create graphviz statemachine file.");
	}

	fprintf (f, "# Generate image with: \"dot -Tpng -o ui_state_machine.png ui_state_machine.dot\"\n");
	fprintf (f, "digraph G {\n");

	for (int i=0;d_ui_state_machine.states[i];++i) {
		struct d_ui_state *state = d_ui_state_machine.states[i];
		fprintf (f, "\t\"%p\" [shape=circle label=\"%s\"];\n", (void*)state, state->description);
	}

	for (int i=0;d_ui_state_machine.transitions[i];++i) {
		struct d_ui_state_transition *transition = d_ui_state_machine.transitions[i];
		fprintf (f, "\t\"%p\" -> \"%p\" [label=\"%s\"];\n",
				 (void*)transition->from, (void*)transition->next,
				 transition->description);
	}

	fprintf (f, "}\n");

	fclose (f);
}

static void
d_ui_sigint (int sig) {
	d_ui->quit = 1;
}

void
d_ui_process_input (int key) {
	struct d_ui_state *state = d_ui_state_current ();
	for (int i=0;state->key_bindings[i].key != 0;++i) {
		if (key == state->key_bindings[i].key) {
			state->key_bindings[i].command->invoke ();
			d_ui->request_redraw = 1;
			break;
		}
	}
}

void
d_ui_step (double now, double delta) {
	struct d_ui_state *state = d_ui_state_current ();
	if (state && state->update) {
		state->update (state, now, delta);
	}
	if (d_ui->request_redraw == 0 && now - d_ui->redraw_last < d_ui->redraw_interval) {
		return;
	}
	d_ui->request_redraw = 1;
	d_ui->redraw_last = now;
}

void
d_ui_render () {
	struct d_ui_state *state = d_ui_state_current ();
	if (state) {
		d_ui->request_redraw = 0;
		d_ui->update_size ();
		if (state->draw) {
			state->draw (state);
		}
		else {
			d_ui->clearscr ();
			d_ui->title_large_draw ();

			struct d_ui_area area = { { d_ui->size.width / 2 - 10, 13}, { 20, 20 } };
			d_ui->menu_draw (&area, state->key_bindings);
		}
	}
}

static char *
d_ui_get_locale_dir () {
	static char directory[PATH_MAX] = { 0 };
	if (directory[0] == 0 ) {
		char tmp[PATH_MAX];
		char tmp2[PATH_MAX];
		getcwd (tmp, PATH_MAX);
		strncpy (tmp2, tmp, PATH_MAX);
		strncat (tmp2, "/dungeons-curses", PATH_MAX);
		if (d_storage_file_exists (tmp2)) {
			strncat (tmp, "/locale", PATH_MAX);
			strncpy (directory, tmp, PATH_MAX);
		}
		else {
			strncpy (directory, "/usr/share/locale/", PATH_MAX);
		}
	}
	return directory;
}

void
d_ui_run () {
	setlocale (LC_ALL, "");
	bindtextdomain ("dungeons", d_ui_get_locale_dir ());
	bind_textdomain_codeset("dungeons", "utf-8");
	textdomain ("dungeons");

	d_ui->init ();
	signal (SIGINT, d_ui_sigint);
	d_ui->run ();
	d_ui->destroy ();
}

struct d_ui_state_transition d_transition_new_world = {
	"New world", &d_main_menu_state, &d_new_world_state };
struct d_ui_state_transition d_transition_new_game = {
	"New game", &d_main_menu_state, &d_gamescreen_state };
struct d_ui_state_transition d_transition_load_game = {
	"Load game", &d_main_menu_state, &d_load_game_state };
struct d_ui_state_transition d_transition_quit_question = {
	"Quit", &d_main_menu_state, &d_quit_state };

struct d_ui_state_transition d_transition_load_game_abort = {
	"Abort", &d_load_game_state, &d_main_menu_state };
struct d_ui_state_transition d_transition_load_game_loading = {
	"Loading", &d_load_game_state, &d_gamescreen_state };

struct d_ui_state_transition d_transition_quit_resume = {
	"Resume", &d_quit_state, &d_main_menu_state };

struct d_ui_state_transition d_transition_esc_menu = {
	"In game menu", &d_gamescreen_state, &d_esc_menu_state };

struct d_ui_state_transition d_transition_esc_menu_resume = {
	"Resume", &d_esc_menu_state, &d_gamescreen_state };
struct d_ui_state_transition d_transition_esc_menu_to_main_menu = {
	"To main menu", &d_esc_menu_state, &d_main_menu_state };

struct d_ui_state_transition d_transition_new_world_back = {
	"To main menu", &d_new_world_state, &d_main_menu_state };

struct d_ui_state_transition d_transition_look_around = {
	"Look around", &d_gamescreen_state, &d_look_around_state };
struct d_ui_state_transition d_transition_look_around_return = {
	"Return", &d_look_around_state, &d_gamescreen_state };

struct d_ui_state_transition d_transition_debug = {
	"Debug", &d_gamescreen_state, &d_debug_state };
struct d_ui_state_transition d_transition_debug_return = {
	"Return", &d_debug_state, &d_gamescreen_state };

struct d_ui_state_machine d_ui_state_machine = {
	"Dungeons",
	&d_main_menu_state,
	{
		&d_debug_state,
		&d_esc_menu_state,
		&d_gamescreen_state,
		&d_load_game_state,
		&d_look_around_state,
		&d_main_menu_state,
		&d_new_world_state,
		&d_quit_state,
		0
	},
	{
		&d_transition_debug,
		&d_transition_debug_return,
		&d_transition_new_game,
		&d_transition_new_world,
		&d_transition_new_world_back,
		&d_transition_load_game,
		&d_transition_load_game_abort,
		&d_transition_load_game_loading,
		&d_transition_look_around,
		&d_transition_look_around_return,
		&d_transition_quit_question,
		&d_transition_quit_resume,
		&d_transition_esc_menu,
		&d_transition_esc_menu_resume,
		&d_transition_esc_menu_to_main_menu,
		0
	}
};
