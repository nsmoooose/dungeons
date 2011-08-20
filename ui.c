#include "curses_main_menu.h"
#include "curses_game.h"
#include "error.h"
#include "ui.h"

extern struct d_ui_state_machine d_ui_state_machine;

void
d_ui_change_state (struct d_ui_state *new_state) {
	for (int i=0;d_ui_state_machine.transitions[i].description != 0;++i) {
		struct d_ui_state_transition *transition = &d_ui_state_machine.transitions[i];
		if (transition->from == d_ui_state_machine.current &&
			transition->next == new_state) {
			d_ui_state_machine.current = new_state;
			return;
		}
	}
	d_bug ("Not allowed to change state!");
}

struct d_ui_state*
d_ui_state_current () {
	return d_ui_state_machine.current;
}

struct d_ui_state_machine d_ui_state_machine = {
	"Dungeons",
	&d_main_menu_state,
	{
		{ "New game", &d_main_menu_state, &d_game_state },
		{ 0, 0, 0}
	}
};

