export CFLAGS+=-std=gnu99 -g -Os -Wall -pedantic -Werror -I$(CURDIR)
export LIBS+=-lrt -lm -lncurses

all: dungeons

dungeons: $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $(CFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

clean:
	$(RM) -r *.o *.gcda *.gcno dungeons

.PHONY: all clean