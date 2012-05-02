export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR) \
	-I$(CURDIR)/src -I$(CURDIR)/curses -I$(CURDIR)/gl -I$(CURDIR)/ui \
	$(shell pkg-config --cflags cairo)
export LIBS_CURSES+=-lrt -lm -lncurses
export LIBS_GL+=-lrt -lm -lncurses -lglut -lGL -lGLU $(shell pkg-config --libs cairo)
export LIBS_TEST+=-lrt -lm -lcheck

FILES_SRC=$(patsubst %.c,%.o,$(wildcard src/*.c))
FILES_DUNGEONS_TESTS=$(patsubst %.c,%.o,$(wildcard src/tests/*.c))
FILES_UI=$(patsubst %.c,%.o,$(wildcard src/ui/*.c))
FILES_CURSES=$(patsubst %.c,%.o,$(wildcard src/ui/curses/*.c))
FILES_GL=$(patsubst %.c,%.o,$(wildcard src/ui/gl/*.c))
FILES_SRV=$(patsubst %.c,%.o,$(wildcard src/srv/*.c))

all: dungeons-curses dungeons-gl dungeons-srv dungeons-tests

dungeons-curses: $(FILES_SRC) $(FILES_UI) $(FILES_CURSES)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_CURSES)

dungeons-gl: $(FILES_SRC) $(FILES_UI) $(FILES_GL)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

dungeons-srv: $(FILES_SRC) $(FILES_SRV)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

dungeons-tests: $(FILES_SRC) $(FILES_DUNGEONS_TESTS)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS) $(LIBS_TEST)

clean:
	$(RM) dungeons-curses dungeons-gl dungeons-srv
	$(RM) $(foreach dir, . src src/ui src/ui/curses src/ui/gl src/srv,$(dir)/*.o $(dir)/*.gcda $(dir)/*.gcno)

tags:
	etags **/*.c **/*.h

.PHONY: all clean tags
