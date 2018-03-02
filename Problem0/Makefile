PROGRAMS = Problem0_a Problem0_b Problem0_c randomFileMaker Problem1 Problem2 Problem3 Problem4

CC = gcc
CFLAGS = -Wall

%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)

