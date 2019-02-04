# vim: tabstop=8 noexpandtab

SOURCE_ID  := $(shell date +%Y%m%d)

PREFIX  = /usr/local
MANDIR  = $(PREFIX)/share/man

CC      = gcc
CFLAGS += -Wall
CFLAGS += -pedantic
CFLAGS += -ansi
CFLAGS += -g
CFLAGS += -ggdb
CFLAGS += -D_BSD_SOURCE
CFLAGS += -pg
CFLAGS += -fprofile-arcs -ftest-coverage

LDFLAGS += -pg
LDFLAGS += -fprofile-arcs -ftest-coverage

SRC     = gc.c secd.c lispkit.c print.c
SRC    += parser.c
SRC    += intern.c
SRC    += main.c

OBJ     = $(SRC:.c=.o)

BIN     = lispkit

COMPILER_SRC = compiler.lisp
COMPILER_BIN = compiler.secd

all: $(BIN) $(BIN)c

.c.o:
	$(CC) -c $(CFLAGS) $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^  $(LDFLAGS)
	@size $(BIN)

install: $(BIN) $(BIN)c
	install -v $(BIN) $(PREFIX)/bin
	install -v $(BIN)c $(PREFIX)/bin
	install -v -d $(MANDIR)/man1
	install -v -m 0644 lispkit.1 $(MANDIR)/man1
	install -v -d $(PREFIX)/share/lispkit
	install -v $(COMPILER_SRC) $(PREFIX)/share/lispkit
	install -v $(COMPILER_BIN) $(PREFIX)/share/lispkit

$(BIN)c: $(BIN)c.in
	sed "s|__COMPILER__=|COMPILER=$(PREFIX)/share/lispkit/$(COMPILER_BIN)|" lispkitc.in > $@
	chmod +x $@

uninstall:
	rm -vf $(PREFIX)/bin/$(BIN)
	rm -vf $(PREFIX)/bin/$(BIN)c
	rm -vr $(MANDIR)/man1/lispkit.1
	rm -vf $(PREFIX)/share/lispkit/$(COMPILER_SRC)
	rm -vf $(PREFIX)/share/lispkit/$(COMPILER_BIN)
	rm -vr $(PREFIX)/share/lispkit

help:
	groff -man -Tascii lispkit.1 | less -R

%.o: %.lisp
	./$(BIN) $(COMPILER_BIN) $< | tee $@

clean:
	rm -f $(OBJ) $(BIN) $(BIN)c *.gcda *.gcno *.gcov *.o examples/*.o

test: $(BIN)
	./test.sh

examples:
	for d in $(shell ls examples/*.lisp); do \
	  ./$(BIN) $(COMPILER_BIN) $$d > $$d.o; \
	done;

bootstrap:
	./$(BIN) $(COMPILER_BIN) $(COMPILER_SRC) > .compiler.out.tmp
	cat $(COMPILER_BIN) | tr '\n' ' ' > .compiler.orig.tmp
	diff -w .compiler.orig.tmp .compiler.out.tmp
	rm .compiler.out.tmp .compiler.orig.tmp

compiler%.secd: PARENT=compiler$$(($* - 1)).secd
compiler%.secd: compiler%.lisp $(PARENT) $(BIN)
	./$(BIN) $(PARENT) $< | fmt -w 80 > $@
	./$(BIN) $@ $< | fmt -w 80 > $@.bootstrap
	diff -y -d $@ $@.bootstrap

compilers: $(addsuffix .secd, $(basename $(wildcard compiler*.lisp)))

archive:
	git archive --prefix=lispkit-$(SOURCE_ID)/ HEAD | gzip > lispkit-$(SOURCE_ID).tar.gz

.PHONY: clean all test archive examples bootstrap

