#include <cairo.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <libintl.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "error.h"
#include "gl_ui.h"

#define d_gl_char_cache_size 1000

struct d_gl_color_def {
	enum d_ui_color index;
	float bg[3];
	float fg[3];
};

struct d_gl_char_texture_cache {
	GLuint textures[d_gl_char_cache_size];
};

static struct d_ui_pos d_gl_pos;
static struct d_ui_size d_gl_char_size = { 8, 16 };
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

static struct d_gl_char_texture_cache d_gl_char_texture_cache[d_last_color];

static void
d_gl_check_errors(char *last_section) {
	GLenum errCode;
	const GLubyte *errString;

	/* Print errors if there are any. */
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error in section: %s, %s\n", last_section, errString);
	}
}

static void
d_gl_create_char_texture (enum d_ui_color color, int key, GLuint texture) {
	cairo_t *cr;
	cairo_surface_t *cairo_surface;
	unsigned char* cairo_data;
	int stride;

	stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, d_gl_char_size.width);
	cairo_data = (unsigned char *) calloc(stride * d_gl_char_size.height, 1);
	cairo_surface = cairo_image_surface_create_for_data(
		cairo_data, CAIRO_FORMAT_ARGB32, d_gl_char_size.width, d_gl_char_size.height, stride);

	cr = cairo_create(cairo_surface);

	cairo_set_source_rgb(cr, d_gl_color_def[color].bg[0],
						 d_gl_color_def[color].bg[1],
						 d_gl_color_def[color].bg[2]);
	cairo_rectangle (cr, 0, 0, d_gl_char_size.width, d_gl_char_size.height);
	cairo_fill (cr);

	cairo_set_source_rgb(cr, d_gl_color_def[color].fg[0],
						 d_gl_color_def[color].fg[1],
						 d_gl_color_def[color].fg[2]);

	cairo_select_font_face(cr, "Monospace",
						   CAIRO_FONT_SLANT_NORMAL,
						   CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size(cr, 12);
	cairo_move_to(cr, 0, d_gl_char_size.height/2 + 3);
	char buffer[5];
	snprintf (buffer, 5, "%c", key);
	cairo_show_text(cr, buffer);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, d_gl_char_size.width, d_gl_char_size.height,
				 0, GL_BGRA, GL_UNSIGNED_BYTE, cairo_data);

	free(cairo_data);
	cairo_destroy(cr);
	cairo_surface_destroy(cairo_surface);

	d_gl_check_errors ("d_gl_create_char_texture");
}

static GLuint
d_gl_char_get_texture (enum d_ui_color color, int key) {
	if (key >= d_gl_char_cache_size || key < 0) {
		d_bug ("Invalid key code: %d", key);
	}
	GLuint texture = d_gl_char_texture_cache[color].textures[key];
	if (glIsTexture (texture) == GL_FALSE) {
		d_gl_create_char_texture (color, key, texture);
	}
	return texture;
}

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

	glutSwapBuffers ();

	d_gl_check_errors ("d_gl_render");
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

	for (int i=0;i<d_last_color;++i) {
		glGenTextures (d_gl_char_cache_size, d_gl_char_texture_cache[i].textures);
	}
}

static void
d_gl_destroy () {
	for (int i=0;i<d_last_color;++i) {
		glDeleteTextures (d_gl_char_cache_size, d_gl_char_texture_cache[i].textures);
	}
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
d_gl_printf_left (int x, int y, const char *format, ...) {
	char buffer[1000];

	va_list args;
	va_start (args, format);
	vsnprintf (buffer, 1000, format, args);
	va_end (args);

	int width = strlen (buffer);

	glEnable(GL_TEXTURE_2D);
	for (int i=0;i<width;++i) {
		int top = y * d_gl_char_size.height;
		int left = (x + i) * d_gl_char_size.width;
		GLuint texture = d_gl_char_get_texture (d_gl_color, buffer[i]);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(left, top);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(left + d_gl_char_size.width, top);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(left + d_gl_char_size.width, top + d_gl_char_size.height);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(left, top + d_gl_char_size.height);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	d_gl_check_errors ("d_gl_printf_left (exit)");
}

static void
d_gl_printf_center (int x, int y, const char *format, ...) {
	char buffer[1000];

	va_list args;
	va_start (args, format);
	vsnprintf (buffer, 1000, format, args);
	va_end (args);

	int width = strlen (buffer);
	d_gl_printf_left (x - width / 2, y, buffer);
}

static void
d_gl_printf_right (int x, int y, const char *format, ...) {
	char buffer[1000];

	va_list args;
	va_start (args, format);
	vsnprintf (buffer, 1000, format, args);
	va_end (args);

	int width = strlen (buffer);
	d_gl_printf_left (x - width, y, buffer);
}

static void
d_gl_box (int x1, int y1, int x2, int y2, char key) {
	for (int i=x1;i<=x2;++i) {
		d_ui->printf_left (i, y1, "%c", key);
		d_ui->printf_left (i, y2, "%c", key);
	}
	for (int i=y1+1;i<y2;++i) {
		d_ui->printf_left (x1, i, "%c", key);
		d_ui->printf_left (x2, i, "%c", key);
	}
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
		                   menu[i].key, gettext (menu[i].command->description));
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

