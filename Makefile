.POSIX:

CC = cc
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Wshadow
CFLAGS += -Wswitch-enum -Wmissing-declarations -Wno-deprecated-declarations
CFLAGS += -Wno-missing-braces
CLIBS =

.PHONY: all

all: iifmt iipost

iifmt: iifmt.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

iipost: iipost.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
