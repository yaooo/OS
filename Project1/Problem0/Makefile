PROGRAMS = Problem0_a Problem0_b Problem0_c randomFileMaker

CC = gcc
CFLAGS = -Wall

%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)

