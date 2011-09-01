#include "curses_io.h"
#include "curses_main_menu.h"
#include "quit.h"

static void d_cmd_quit_accept_cb ();
static void d_cmd_quit_decline_cb ();

struct d_ui_command d_cmd_quit_accept = { "Quit", d_cmd_quit_accept_cb };
struct d_ui_command d_cmd_quit_decline = { "Resume", d_cmd_quit_decline_cb };

struct d_ui_state d_quit_state = {
	"Quit",
	0,
	0,
	0,
	{
		{ 'y', &d_cmd_quit_accept },
		{ 'n', &d_cmd_quit_decline },
		{ 0, 0 }
	}
};

static void
d_cmd_quit_accept_cb () {
	d_ui->quit = 1;
}

static void
d_cmd_quit_decline_cb () {
	d_ui_do_transition (&d_transition_quit_resume);
}
