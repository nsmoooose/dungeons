export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR) \
	-I$(CURDIR)/src -I$(CURDIR)/curses -I$(CURDIR)/gl -I$(CURDIR)/ui \
	$(shell pkg-config --cflags cairo)
export LIBS_CURSES+=-lrt -lm -lncursesw
export LIBS_GL+=-lrt -lm -lglut -lGL -lGLU $(shell pkg-config --libs cairo)
export LIBS_TEST+=-lrt -lm -lcheck

ifdef COVERAGE
	CFLAGS+=--coverage
endif

FILES_SRC=$(patsubst %.c,%.o,$(wildcard src/*.c))
FILES_DUNGEONS_TESTS=$(patsubst %.c,%.o,$(wildcard src/tests/*.c))
FILES_UI=$(patsubst %.c,%.o,$(wildcard src/ui/*.c))
FILES_CURSES=$(patsubst %.c,%.o,$(wildcard src/ui/curses/*.c))
FILES_GL=$(patsubst %.c,%.o,$(wildcard src/ui/gl/*.c))
FILES_SRV=$(patsubst %.c,%.o,$(wildcard src/srv/*.c))

FILES_INTL_MO=$(patsubst %.po,%.mo,$(wildcard locale/*/LC_MESSAGES/*.po))
FILES_INTL_PO=$(wildcard locale/*/LC_MESSAGES/*.po)

all: dungeons-curses dungeons-gl dungeons-srv dungeons-tests intl

dungeons-curses: $(FILES_SRC) $(FILES_UI) $(FILES_CURSES)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_CURSES)

dungeons-gl: $(FILES_SRC) $(FILES_UI) $(FILES_GL)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

dungeons-srv: $(FILES_SRC) $(FILES_SRV)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS_GL)

dungeons-tests: $(FILES_SRC) $(FILES_DUNGEONS_TESTS)
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS) $(LIBS_TEST)

clean:
	$(RM) -r coverage dungeons-curses dungeons-gl dungeons-srv
	$(RM) $(foreach dir, . src src/ui src/ui/curses src/ui/gl src/srv,$(dir)/*.o $(dir)/*.gcda $(dir)/*.gcno)
	$(RM) locale/*/LC_MESSAGES/*.mo

coverage: clean
	@$(MAKE) -C . all COVERAGE=1
	@mkdir coverage
	@-./dungeons-tests
	@lcov -b . --directory . --capture --output-file coverage/coverage.info
	@genhtml -o coverage/ coverage/coverage.info

intl: $(FILES_INTL_MO)

locale/dungeons.pot: $(wildcard src/ui/*.c)
	xgettext -a -d dungeons -s -o locale/dungeons.pot src/ui/*.c

$(FILES_INTL_PO): locale/dungeons.pot
	msgmerge -s -U $@ locale/dungeons.pot

%.mo: %.po
	msgfmt -c -v -o $@ $^

.PHONY: all clean intl
