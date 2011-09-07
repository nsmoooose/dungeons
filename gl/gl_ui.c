#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#include "clock.h"
#include "gl_ui.h"

static void
d_gl_window_size_change(int width, int height) {
	d_ui->size.width = width / 8;
	d_ui->size.height = height / 15;
	glViewport(0, 0, width, height);
}

static void
d_gl_idle () {
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
	/* TODO clear depth buffers etc ? */

	d_ui_render ();

	/* TODO swap buffers etc */
	glutSwapBuffers ();
}

static void
d_gl_init () {
	int argc = 1;
	char *argv[] = { "dungeons-gl" };
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Dungeons");
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
}

static void
d_gl_printf_center (int x, int y, const char *format, ...) {
}

static void
d_gl_printf_left (int x, int y, const char *format, ...) {
}

static void
d_gl_printf_right (int x, int y, const char *format, ...) {
}

static void
d_gl_box (int x1, int y1, int x2, int y2, char key) {
}

static void
d_gl_update_size () {
}

static void
d_gl_set_color (enum d_ui_color pair) {
}

static void
d_gl_hide_cursor () {
}

static void
d_gl_show_cursor () {
}

static void
d_gl_widget_title_large_draw () {
}

static void
d_gl_widget_menu_draw (struct d_ui_area *area, struct d_ui_key_binding menu[]) {
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
