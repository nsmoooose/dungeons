#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "gl_ui.h"

struct d_gl_color_def {
	enum d_ui_color index;
	float bg[3];
	float fg[3];
};

static struct d_ui_pos d_gl_pos;
static struct d_ui_size d_gl_char_size = { 8, 15 };
static enum d_ui_color d_gl_color;
static struct d_gl_color_def d_gl_color_def[] = {
	{ 0, { 0, 0, 0 }, { 0, 0, 0 } },
	{ d_black_white, { 0, 0, 0 }, { 1, 1, 1 } },
	{ d_white_black, { 1, 1, 1 }, { 0, 0, 0 } },
	{ d_black_green, { 0, 0, 0 }, { 0, 1, 0 } },
	{ d_green_white, { 0, 1, 0 }, { 1, 1, 1 } },
	{ d_cyan_white, { 0, 1, 1 }, { 1, 1, 1 } },
	{ d_black_blue, { 0, 0, 0 }, { 0, 0, 1 } },
	{ d_black_cyan, { 0, 0, 0 }, { 0, 1, 1 } }
};

static void
d_gl_window_size_change(int width, int height) {
	d_ui->size.width = width / d_gl_char_size.width;
	d_ui->size.height = height / d_gl_char_size.height;
	glViewport(0, 0, width, height);
}

static void
d_gl_idle () {
	if (d_ui->quit) {
		glutLeaveMainLoop ();
	}
	static double last = 0;
	double now = d_time_get ();

	double delta = now - last;
	d_ui_step (now, delta);

	if (d_ui->request_redraw) {
		glutPostRedisplay ();
	}
	last = now;
}

static void
d_gl_render () {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0.0f, viewport[2], viewport[3], 0.0f, -200.0f, 200.0f);

	d_ui_render ();

	/* TODO swap buffers etc */
	glutSwapBuffers ();
}

static void
d_gl_key_press(unsigned char key, int x, int y) {
	d_ui_process_input (key);
}

static void
d_gl_init () {
	int argc = 1;
	char *argv[] = { "dungeons-gl" };
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Dungeons");
	glutKeyboardFunc(d_gl_key_press);
	glutReshapeFunc(d_gl_window_size_change);
	glutIdleFunc(d_gl_idle);
	glutDisplayFunc(d_gl_render);
}

static void
d_gl_destroy () {
}

static void
d_gl_run () {
	glutMainLoop ();
}

static void
d_gl_clear () {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

static void
d_gl_setpos (int x, int y) {
	d_gl_pos.x = x;
	d_gl_pos.y = y;
}

static void
d_gl_printf_center (int x, int y, const char *format, ...) {
	int width = strlen (format);
	int cx = fmax (x - (width / 2), 0);
	int cy = y;

	glColor3fv (d_gl_color_def[d_gl_color].fg);
	glRecti (cx*d_gl_char_size.width, cy*d_gl_char_size.height,
			 (cx+1+width)*d_gl_char_size.width, (cy+1)*d_gl_char_size.height);
}

static void
d_gl_printf_left (int x, int y, const char *format, ...) {
	int width = strlen (format);
	glColor3fv (d_gl_color_def[d_gl_color].fg);
	glRecti (x*d_gl_char_size.width, y*d_gl_char_size.height,
			 (x+1+width)*d_gl_char_size.width, (y+1)*d_gl_char_size.height);
}

static void
d_gl_printf_right (int x, int y, const char *format, ...) {
	int width = strlen (format);
	int cx = x - (width);
	int cy = y;

	glColor3fv (d_gl_color_def[d_gl_color].fg);
	glRecti (cx*d_gl_char_size.width, cy*d_gl_char_size.height,
			 (cx+1+width)*d_gl_char_size.width, (cy+1)*d_gl_char_size.height);
}

static void
d_gl_box (int x1, int y1, int x2, int y2, char key) {
}

static void
d_gl_update_size () {
}

static void
d_gl_set_color (enum d_ui_color pair) {
	d_gl_color = pair;
}

static void
d_gl_hide_cursor () {
}

static void
d_gl_show_cursor () {
}

static void
d_gl_widget_title_large_draw () {
	const static char *title[] = {
		"DDDDD   U    U N    N  GGGG  EEEEE  OOOO   N    N  SSSS ",
		"D    D  U    U NN   N G    G E     O    O  NN   N S    S",
		"D     D U    U N N  N G      E     O    O  N N  N S     ",
		"D     D U    U N N  N G  GG  EEEE  O    O  N N  N  SSSS ",
		"D     D U    U N  N N G    G E     O    O  N  N N      S",
		"D    D  U    U N   NN G    G E     O    O  N   NN S    S",
		"DDDDD    UUUU  N    N  GGGG  EEEEE  OOOO   N    N  SSSS "
	};
	d_ui->set_color (d_black_green);

	int center = d_ui->size.width / 2;

	for (int i=0;i<7;++i) {
		d_ui->printf_center (center, 3 + i, title[i]);
	}
}

static void
d_gl_widget_menu_draw (struct d_ui_area *area, struct d_ui_key_binding menu[]) {
	d_ui->set_color (d_black_white);

	for (int i=0,row=area->pos.y;menu[i].key != 0;++i,++row) {
		d_ui->printf_left (area->pos.x, row, "%c) %s",
						   menu[i].key, menu[i].command->description);
	}
}

struct d_ui d_gl_ui_implementation = {
	0,          /* quit flag */
	{ 0, 0 },   /* size */
	0.0,        /* last redraw */
	1.0,        /* redraw interval */
	0,          /* request redraw flag */
	d_gl_init,
	d_gl_destroy,
	d_gl_run,
	d_gl_clear,
	d_gl_setpos,
	d_gl_printf_center,
	d_gl_printf_left,
	d_gl_printf_right,
	d_gl_box,
	d_gl_update_size,
	d_gl_set_color,
	d_gl_hide_cursor,
	d_gl_show_cursor,

	d_gl_widget_title_large_draw,
	d_gl_widget_menu_draw
};
