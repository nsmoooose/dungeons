export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR) \
	-I$(CURDIR)/src -I$(CURDIR)/curses -I$(CURDIR)/gl
export LIBS+=-lrt -lm -lncurses

vpath %.c src
vpath %.c curses
vpath %.c gl

FILES_SRC=$(patsubst %.c,%.o,$(notdir $(wildcard src/*.c)))
FILES_CURSES=$(patsubst %.c,%.o,$(notdir $(wildcard curses/*.c)))
FILES_GL=$(patsubst %.c,%.o,$(notdir $(wildcard gl/*.c)))

all: dungeons-curses

dungeons-curses: $(FILES_SRC) $(FILES_CURSES)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

dungeons-gl: $(FILES_SRC) $(FILES_GL)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

clean:
	$(RM) -r *.o *.gcda *.gcno dungeons-curses dungeons-gl

.PHONY: all clean