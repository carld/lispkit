# vim: tabstop=8 noexpandtab

SOURCE_ID  := $(shell date +%Y%m%d)

CC      = gcc
CFLAGS += -Wall
CFLAGS += -pedantic
CFLAGS += -ansi
CFLAGS += -g
CFLAGS += -ggdb
CFLAGS += -D_BSD_SOURCE

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
	rm -f $(OBJ) $(BIN) *.gcda *.gcno *.gcov *.o examples/*.o

test:
	./test.sh

archive:
	git archive --prefix=lispkit-$(SOURCE_ID)/ HEAD | gzip > lispkit-$(SOURCE_ID).tar.gz

.PHONY: clean all test archive

