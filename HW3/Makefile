PROGRAMS = philosopher_mutex_cond philosopher_monitor

CC = gcc
CFLAGS = -pthread -lm -Wall
%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)

