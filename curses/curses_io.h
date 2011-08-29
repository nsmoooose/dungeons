#ifndef __DUNGEONS_CURSES_IO_H__
#define __DUNGEONS_CURSES_IO_H__


extern int d_quit;

extern struct d_curses_size {
	int width;
	int height;
} d_curses_size;

extern void (*d_curses_update) (double now, double delta);
extern void (*d_curses_draw) ();
extern void (*d_curses_key) (char key);


void d_curses_step (double now, double delta);
void d_curses_run ();

#endif
