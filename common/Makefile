CC = gcc

SRCDIR = ./lib
SOURCES = $(wildcard $(SRCDIR)/*.c)
EXAMPLE = example.c
BLOCK_TEST = block_test.c
OUT = example.out
OUT_BLOCK_TEST = block_test.out

all: $(SOURCES) $(EXAMPLE)
	$(CC) $(CFLAGS) $(SOURCES) $(EXAMPLE) -o $(OUT)

block-test: $(SOURCES) $(BLOCK_TEST)
	$(CC) $(CFLAGS) $(SOURCES) $(BLOCK_TEST) -o $(OUT_BLOCK_TEST)

.PHONY: clean

clean:
	rm -f $(OUT)

