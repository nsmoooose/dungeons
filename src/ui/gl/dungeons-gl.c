#include "../../memory.h"
#include "../ui.h"
#include "gl_ui.h"

int
main (int argc, char *argv[]) {
	d_ui = &d_gl_ui_implementation;
	d_ui_run ();
	d_print_memory_leaks ();
	return 0;
}
