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

test: $(BIN)
	./test.sh

examples:
	for d in `ls examples/*.lisp`; do \
	  ./$(BIN) compiler.ascii $$d > $$d.o; \
	done;

bootstrap:
	./$(BIN) compiler.ascii compiler.txt.ascii > .compiler.out.tmp
	cat compiler.ascii | tr '\n' ' ' > .compiler.orig.tmp
	diff -w .compiler.orig.tmp .compiler.out.tmp
	rm .compiler.out.tmp .compiler.orig.tmp

archive:
	git archive --prefix=lispkit-$(SOURCE_ID)/ HEAD | gzip > lispkit-$(SOURCE_ID).tar.gz

.PHONY: clean all test archive examples

