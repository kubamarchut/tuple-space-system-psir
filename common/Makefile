CC = gcc

SRCDIR = ./lib
SOURCES = $(wildcard $(SRCDIR)/*.c)
EXAMPLE = example.c
BLOCK_TEST = block_test.c
TUPLE_INJECTOR= tuple_injector.c
OUT = example.out
OUT_BLOCK_TEST = block_test.out
OUT_TUPLE_INJECTOR = tuple_injector.out

all: $(SOURCES) $(EXAMPLE)
	$(CC) $(CFLAGS) $(SOURCES) $(EXAMPLE) -o $(OUT)

block-test: $(SOURCES) $(BLOCK_TEST)
	$(CC) $(CFLAGS) $(SOURCES) $(BLOCK_TEST) -o $(OUT_BLOCK_TEST)

tuple-injector: $(SOURCES) $(TUPLE_INJECTOR)
	$(CC) $(CFLAGS) $(SOURCES) $(TUPLE_INJECTOR) -o $(OUT_TUPLE_INJECTOR)

.PHONY: clean

clean:
	rm -f $(OUT)

