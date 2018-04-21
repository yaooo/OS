PROGRAMS = Problem2 mypthread_test Part1a Part1b Part1c

CC = gcc
CFLAGS = -pthread -lm -Wall
%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)


