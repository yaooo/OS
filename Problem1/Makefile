<<<<<<< HEAD
PROGRAMS =Problem1
=======
PROGRAMS = Problem1
>>>>>>> d18a9a437a99de068f3fae5728a3c8f3b627746c

CC = gcc
CFLAGS = -Wall

%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)

