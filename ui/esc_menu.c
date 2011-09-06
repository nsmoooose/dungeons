#include "esc_menu.h"
#include "game.h"
#include "gamescreen.h"

static void d_cmd_resume_game_cb ();
static void d_cmd_abandon_dungeon_cb ();
static void d_cmd_save_dungeon_cb ();

struct d_ui_command d_cmd_resume_game = { "Resume game", d_cmd_resume_game_cb };
struct d_ui_command d_cmd_abandon_dungeon = { "Abandon dungeon", d_cmd_abandon_dungeon_cb };
struct d_ui_command d_cmd_save_dungeon = { "Save dungeon", d_cmd_save_dungeon_cb };

struct d_ui_state d_esc_menu_state = {
	"In game menu",
	0,
	0,
	0,
	{
		{ 'r', &d_cmd_resume_game },
		{ 'a', &d_cmd_abandon_dungeon },
		{ 's', &d_cmd_save_dungeon },
		{ 0, 0 }
	}
};

void
d_cmd_resume_game_cb () {
	d_ui_do_transition (&d_transition_esc_menu_resume);
}

void
d_cmd_abandon_dungeon_cb () {
	/* TODO abandon the dungeon completely by removing from disc. */
	d_game_context_destroy (d_context);
	d_context = 0;
	d_ui_do_transition (&d_transition_esc_menu_to_main_menu);
}

void
d_cmd_save_dungeon_cb () {
	/* TODO Save dungeon before returning to the main menu. */
	d_game_context_destroy (d_context);
	d_context = 0;
	d_ui_do_transition (&d_transition_esc_menu_to_main_menu);
}
