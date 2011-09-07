export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR) \
	-I$(CURDIR)/src -I$(CURDIR)/curses -I$(CURDIR)/gl -I$(CURDIR)/ui
export LIBS_CURSES+=-lrt -lm -lncurses
export LIBS_GL+=-lrt -lm -lncurses -lglut -lGL -lGLU

vpath %.c src
vpath %.c curses
vpath %.c gl
vpath %.c ui

FILES_SRC=$(patsubst %.c,%.o,$(notdir $(wildcard src/*.c)))
FILES_UI=$(patsubst %.c,%.o,$(notdir $(wildcard ui/*.c)))
FILES_CURSES=$(patsubst %.c,%.o,$(notdir $(wildcard curses/*.c)))
FILES_GL=$(patsubst %.c,%.o,$(notdir $(wildcard gl/*.c)))

all: dungeons-curses dungeons-gl

dungeons-curses: $(FILES_SRC) $(FILES_UI) $(FILES_CURSES)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_CURSES)

dungeons-gl: $(FILES_SRC) $(FILES_UI) $(FILES_GL)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

clean:
	$(RM) -r *.o *.gcda *.gcno dungeons-curses dungeons-gl

tags:
	etags **/*.c **/*.h

.PHONY: all clean tags
