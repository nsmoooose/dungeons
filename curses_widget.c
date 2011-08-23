#include <math.h>
#include <string.h>

#include "curses_io.h"
#include "curses_widget.h"
#include "memory.h"

void
d_curses_widget_title_large_draw () {
	const static char *title[] = {
		"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
		"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
		"D     D U    U N N  N G      E     O    O  N N  N S     ",
		"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
		"D     D U    U N  N N G    G E     O    O  N  N N      S",
		"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
		"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
	};
	d_curses_set_color (d_black_green);

	int center = d_curses_size.width / 2;

	for (int i=0;i<7;++i) {
		d_curses_printf_center (center, 3 + i, title[i]);
	}
}

void
d_curses_widget_menu_draw (struct d_ui_area *area, struct d_ui_key_binding menu[]) {
	d_curses_set_color (d_black_white);

	for (int i=0,row=area->pos.y;menu[i].key != 0;++i,++row) {
		d_curses_printf_left (area->pos.x, row, "%c) %s",
							  menu[i].key, menu[i].command->description);
	}
}
