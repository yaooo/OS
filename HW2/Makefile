PROGRAMS = Problem1 Modified_problem1 Problem2 Problem9

CC = gcc
CFLAGS = -pthread -lm
%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)

