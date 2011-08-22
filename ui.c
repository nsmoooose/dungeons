#include <stdio.h>

#include "curses_esc_menu.h"
#include "curses_game.h"
#include "curses_main_menu.h"
#include "curses_quit.h"
#include "error.h"
#include "ui.h"

extern struct d_ui_state_machine d_ui_state_machine;

void
d_ui_do_transition (struct d_ui_state_transition *transition) {
	for (int i=0;d_ui_state_machine.transitions[i];++i) {
		if (transition == d_ui_state_machine.transitions[i]) {
			if (transition->from == d_ui_state_current ()) {
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
		fprintf (f, "\t\"%x\" [shape=circle label=\"%s\"];\n", (unsigned int)state, state->description);
	}

	for (int i=0;d_ui_state_machine.transitions[i];++i) {
		struct d_ui_state_transition *transition = d_ui_state_machine.transitions[i];
		fprintf (f, "\t\"%x\" -> \"%x\" [label=\"%s\"];\n",
				 (unsigned int)transition->from, (unsigned int)transition->next,
				 transition->description);
	}

	fprintf (f, "}\n");

	fclose (f);
}

struct d_ui_state_transition d_transition_new_game = { "New game", &d_main_menu_state, &d_game_state };
struct d_ui_state_transition d_transition_quit_question = { "Quit", &d_main_menu_state, &d_quit_state };
struct d_ui_state_transition d_transition_quit_resume = { "Resume", &d_quit_state, &d_main_menu_state };

struct d_ui_state_transition d_transition_esc_menu = { "In game menu", &d_game_state, &d_esc_menu_state };
struct d_ui_state_transition d_transition_esc_menu_resume = { "Resume", &d_esc_menu_state, &d_game_state };
struct d_ui_state_transition d_transition_esc_menu_to_main_menu = { "To main menu", &d_esc_menu_state, &d_main_menu_state};


struct d_ui_state_machine d_ui_state_machine = {
	"Dungeons",
	&d_main_menu_state,
	{
		&d_esc_menu_state,
		&d_game_state,
		&d_main_menu_state,
		&d_quit_state,
		0
	},
	{
		&d_transition_new_game,
		&d_transition_quit_question,
		&d_transition_quit_resume,
		&d_transition_esc_menu,
		&d_transition_esc_menu_resume,
		&d_transition_esc_menu_to_main_menu,
		0
	}
};
