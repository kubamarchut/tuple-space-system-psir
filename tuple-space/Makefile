CC = gcc

SRCDIR = ../common/lib
SOURCES = $(wildcard $(SRCDIR)/*.c)
EXAMPLE = main.c
OUT = tuple.out

all: $(SOURCES) $(EXAMPLE)
	$(CC) $(CFLAGS) $(SOURCES) $(EXAMPLE) -o $(OUT)

.PHONY: clean

clean:
	rm -f $(OUT)
