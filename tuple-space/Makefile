CC = gcc

SRCDIR = ../common/lib
SRCDIR2 = ./lib
SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES2 = $(wildcard $(SRCDIR2)/*.c)
EXAMPLE = main.c
OUT = tuple.out

all: $(SOURCES) $(EXAMPLE)
	$(CC) $(CFLAGS) $(SOURCES) $(SOURCES2) $(EXAMPLE) -o $(OUT)

.PHONY: clean

clean:
	rm -f $(OUT)
