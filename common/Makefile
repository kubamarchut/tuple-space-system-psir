CC = gcc

SRCDIR = ./lib
SOURCES = $(wildcard $(SRCDIR)/*.c)
EXAMPLE = example.c
OUT = example.out

all: $(SOURCES) $(EXAMPLE)
	$(CC) $(CFLAGS) $(SOURCES) $(EXAMPLE) -o $(OUT)

.PHONY: clean

clean:
	rm -f $(OUT)
