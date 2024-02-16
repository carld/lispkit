/*
  A Lispkit implementation.

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
struct Symbol { const char * symbol; };
struct Cons   { Object * car; Object * cdr; };

union Object {
  struct Number Number;
  struct Symbol Symbol;
  struct Cons   Cons;
};

extern Object * _stack;
extern Object * _environ;
extern Object * _control;
extern Object * _dump;

extern Object * _true;
extern Object * _false;
extern Object * _nil;

extern Object * _work;

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

void init(void);
Object * execute(Object *fn, Object *args);

const char * intern_string(char *string);
void intern_free(void);

void exp_print(Object *);
const char * type_str(int);

Object * get_exp(FILE *);
Object * get_exp_list(FILE *);

#endif

