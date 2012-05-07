#include <stdio.h>

#include "curses_ui.h"
#include "../../memory.h"
#include "../ui.h"

int
main (int argc, char *argv[]) {
	d_ui_state_machine_print ("ui_state_machine.dot");

	d_ui = &d_curses_ui_implementation;
	d_ui_run ();

	d_print_memory_leaks ();
	return 0;
}
