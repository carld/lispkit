# vim: tabstop=8 noexpandtab

CC      = gcc
CFLAGS += -Wall
CFLAGS += -pedantic
CFLAGS += -ansi
CFLAGS += -g
CFLAGS += -ggdb

SRC     = gc.c secd.c lispkit.c print.c
SRC    += parser.c
SRC    += intern.c
SRC    += tokenizer.c
SRC    += main.c
OBJ     = $(SRC:.c=.o)
BIN     = lispkit

all: $(BIN)

.c.o:
	$(CC) -c $(CFLAGS) $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^  $(LDFLAGS)
	@size $(BIN)

%.o: %.lisp
	./$(BIN) compiler.ascii $< | tee $@

clean:
	rm -f $(OBJ) $(BIN) *.gcda *.gcno *.gcov *.o

test:
	./test.sh

.PHONY: clean all test

