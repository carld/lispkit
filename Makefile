# vim: tabstop=8 noexpandtab

CC      = gcc
CFLAGS += -Wall 
CFLAGS += -pedantic 
CFLAGS += -ansi 
CFLAGS += -g 
CFLAGS += -ggdb 

SRC     = gc.c secd.c lispkit.c parser.c print.c 
SRC    += main.c
OBJ     = $(SRC:.c=.o)
BIN     = lispkit

all: $(BIN) 

.c.o:
	$(CC) -c $(CFLAGS) $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^  $(LDFLAGS)

clean:
	rm -f $(OBJ) $(BIN) *.gcda *.gcno *.gcov *.o 

.PHONY: clean default all 
