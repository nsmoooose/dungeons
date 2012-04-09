export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR) \
	-I$(CURDIR)/src -I$(CURDIR)/curses -I$(CURDIR)/gl -I$(CURDIR)/ui \
	$(shell pkg-config --cflags cairo)
export LIBS_CURSES+=-lrt -lm -lncurses
export LIBS_GL+=-lrt -lm -lncurses -lglut -lGL -lGLU $(shell pkg-config --libs cairo)

FILES_SRC=$(patsubst %.c,%.o,$(wildcard src/*.c))
FILES_UI=$(patsubst %.c,%.o,$(wildcard ui/*.c))
FILES_CURSES=$(patsubst %.c,%.o,$(wildcard curses/*.c))
FILES_GL=$(patsubst %.c,%.o,$(wildcard gl/*.c))
FILES_SRV=$(patsubst %.c,%.o,$(wildcard srv/*.c))

all: dungeons-curses dungeons-gl dungeons-srv

dungeons-curses: $(FILES_SRC) $(FILES_UI) $(FILES_CURSES)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_CURSES)

dungeons-gl: $(FILES_SRC) $(FILES_UI) $(FILES_GL)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

dungeons-srv: $(FILES_SRC) $(FILES_SRV)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

clean:
	$(RM) dungeons-curses dungeons-gl dungeons-srv
	$(RM) $(foreach dir, . src ui curses gl srv,$(dir)/*.o $(dir)/*.gcda $(dir)/*.gcno)

tags:
	etags **/*.c **/*.h

.PHONY: all clean tags
