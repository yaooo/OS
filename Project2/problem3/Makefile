CC=gcc
CFLAGS=-g

DEMO=mtsort

default: help

all: ult system

help:
	@echo "We can build in two ways (each of these will build $(DEMO).c):"
	@echo "make ult - Build $(DEMO) with your ULT implementation."
	@echo "make system - Build the $(DEMO) using the system pthreads implementation."

ult: $(DEMO).c mypthread.c mypthread.h
	$(CC) $(CFLAGS) $(DEMO).c mypthread.c -o $(DEMO)-ult

system: $(DEMO).c
	$(CC) $(CFLAGS) $(DEMO).c -o $(DEMO)-system -pthread -include mypthread-system-override.h

clean:
	@echo "Cleaning for ult..."
	@rm -f $(DEMO)-ult $(DEMO)-system



