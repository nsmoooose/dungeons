#include "curses_game.h"
#include "curses_main_menu.h"
#include "curses_quit.h"
#include "error.h"
#include "ui.h"

extern struct d_ui_state_machine d_ui_state_machine;

void
d_ui_do_transition (struct d_ui_state_transition *transition) {
	for (int i=0;d_ui_state_machine.transitions[i]->description != 0;++i) {
		if (transition == d_ui_state_machine.transitions[i]) {
			if (transition->from == d_ui_state_current ()) {
				d_ui_state_machine.current = transition->next;
			}
			else {
				d_bug ("Cannot perform transition: %s", transition->description);
			}
		}
	}
	d_bug ("Transition not found in state machine.");
}

struct d_ui_state*
d_ui_state_current () {
	return d_ui_state_machine.current;
}

struct d_ui_state_transition d_transition_new_game = { "New game", &d_main_menu_state, &d_game_state };
struct d_ui_state_transition d_transition_quit_question = { "Quit", &d_main_menu_state, &d_quit_state };
struct d_ui_state_transition d_transition_quit_resume = { "Resume", &d_quit_state, &d_main_menu_state };

struct d_ui_state_machine d_ui_state_machine = {
	"Dungeons",
	&d_main_menu_state,
	{
		&d_transition_new_game,
		&d_transition_quit_question,
		&d_transition_quit_resume,
		0
	}
};

