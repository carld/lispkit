/*
  A Lispkit Lisp implementation.

  Copyright (c) 2011  A. Carl Douglas

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/* vim: softtabstop=2 shiftwidth=2 expandtab  */
#ifndef SECD_H
#define SECD_H 1

union Object;
typedef union Object Object;

enum { NUMBER = 1, SYMBOL = 2, CONS = 3 };

struct Number { long number; };
struct Symbol { const char *symbol; };
struct Cons   { Object *car; Object *cdr; };

union Object {
  struct Number Number;
  struct Symbol Symbol;
  struct Cons   Cons;
};

extern Object *stack;
extern Object *environ;
extern Object *control;
extern Object *dump;

extern Object *_true;
extern Object *_false;
extern Object *_nil;

extern Object *work;

Object * cons(Object *, Object *);
Object * car(Object *);
Object * cdr(Object *);

Object * number(int);
Object * symbol(char *);

const char * string_value(Object *);
int number_value(Object *);

int is_number(Object *);
int is_symbol(Object *);
int is_atom(Object *);

void init();
Object * execute(Object *fn, Object *args);

const char * intern_string(char *string);
void intern_free(void);

void print(Object *);

struct Token {
  char * file;
  char * token;
  unsigned line;
  unsigned word;
};

void scanner(void);
void tokenize(FILE *);
void tokenizer_free(void);

Object * get_exp(FILE *);
Object * get_exp_list(FILE *);

enum {
  OP_LD   =  1,
  OP_LDC  =  2,
  OP_LDF  =  3,
  OP_AP   =  4,
  OP_RTN  =  5,
  OP_DUM  =  6,
  OP_RAP  =  7,
  OP_SEL  =  8,
  OP_JOIN =  9,
  OP_CAR  = 10,
  OP_CDR  = 11,
  OP_ATOM = 12,
  OP_CONS = 13,
  OP_EQ   = 14,
  OP_ADD  = 15,
  OP_SUB  = 16,
  OP_MUL  = 17,
  OP_DIV  = 18,
  OP_REM  = 19,
  OP_LEQ  = 20,
  OP_STOP = 21
};

#endif

