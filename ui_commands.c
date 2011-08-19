/* TODO move d_quit flag outside of curses */
#include "curses_io.h"
#include "ui.h"
#include "ui_commands.h"

void d_cmd_quit_cb ();

struct d_ui_command d_cmd_quit = { "Quit", d_cmd_quit_cb };

void
d_cmd_quit_cb () {
	d_quit = 1;
}
