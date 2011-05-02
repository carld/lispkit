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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

typedef void (*OpCode) (void);

Object *stack;
Object *environ;
Object *control;
Object *dump;

Object *true;
Object *false;
Object *nil;

Object *work;

Object *strings;

static int stopped = 0;


void ldc() {
  stack   = cons(car(cdr(control)), stack);
  control = cdr(cdr(control));
}

void ld() {
  int i = 0;
  work    = environ;
  for (i = 1; i <= number_value(car(car(cdr(control)))); i++) {
    work = cdr(work);
  }
  work    = car(work);
  for (i = 1; i <= number_value(cdr(car(cdr(control)))); i++) {
    work = cdr(work);
  }
  work    = car(work);
  stack   = cons(work, stack);
  control = cdr(cdr(control));
}

void _car() {
  stack   = cons(car(car(stack)), cdr(stack));
  control = cdr(control);
}

void _cdr() {
  stack   = cons(cdr(car(stack)), cdr(stack));
  control = cdr(control);
}

void atom() {
  if (is_number(car(stack)) || is_symbol(car(stack))) {
    stack = cons(true, cdr(stack));
  } else {
    stack = cons(false, cdr(stack));
  }
  control = cdr(control);
}

void _cons() {
  stack   = cons(cons(car(stack),car(cdr(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void sub() {
  stack   = cons(number(number_value(car(cdr(stack))) - number_value(car(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void add() {
  stack   = cons(number(number_value(car(cdr(stack))) + number_value(car(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void mul() {
  stack   = cons(number(number_value(car(cdr(stack))) * number_value(car(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void _div() {
  stack   = cons(number(number_value(car(cdr(stack))) / number_value(car(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void rem() {
  stack   = cons(number(number_value(car(cdr(stack))) % number_value(car(stack))),cdr(cdr(stack)));
  control = cdr(control);
}

void leq() {
  if (number_value(car(cdr(stack))) <= number_value(car(stack))) {
    stack = cons(true, cdr(cdr(stack)));
  } else {
    stack = cons(false, cdr(cdr(stack)));
  }
  control = cdr(control);
}

void eq() {
  if ((is_symbol(car(stack)) && is_symbol(car(cdr(stack))) && (strcmp(string_value(car(stack)), string_value(car(cdr(stack)))) == 0)) ||
    (is_number(car(stack)) && is_number(car(cdr(stack))) && number_value(car(stack)) == number_value(car(cdr(stack))))) {
    stack = cons(true, cdr(cdr(stack)));
  } else {
    stack = cons(false, cdr(cdr(stack)));
  }
  control = cdr(control);
}

void ldf() {
  stack = cons(cons(car(cdr(control)), environ), stack);
  control = cdr(cdr(control));
}

void rtn() {
  stack   = cons(car(stack), car(dump));
  environ = car(cdr(dump));
  control = car(cdr(cdr(dump)));
  dump    = cdr(cdr(cdr(dump)));
}

void dum() {
  environ = cons(nil, environ);
  control = cdr(control);
}

void rap() {
  dump         = cons(cdr(cdr(stack)), cons(cdr(environ), cons(cdr(control),dump)));
  environ      = cdr(car(stack));
  environ->Cons.car = car(cdr(stack));
  control      = car(car(stack));
  stack        = nil;
}

void sel() {
  dump = cons(cdr(cdr(cdr(control))),dump);
  if (strcmp(string_value(car(stack)), string_value(true)) == 0) {
    control = car(cdr(control));
  } else {
    control = car(cdr(cdr(control)));
  }
  stack   = cdr(stack);
}

void join() {
  control = car(dump);
  dump    = cdr(dump);
}

void ap() {
  dump    = cons(cdr(cdr(stack)),cons(environ,cons(cdr(control), dump)));
  environ = cons(car(cdr(stack)),cdr(car(stack)));
  control = car(car(stack));
  stack   = nil;
}

void _stop() {
  stopped = 1;
}

OpCode op_code[] = {
  0,
  ld,
  ldc,
  ldf,
  ap,
  rtn,
  dum,
  rap,
  sel,
  join,
  _car,
  _cdr,
  atom,
  _cons,
  eq,
  add,
  sub,
  mul,
  _div,
  rem,
  leq,
  _stop
};



Object * execute(Object *fn, Object *args)
{

  stack      = cons(args, nil);
  environ    = nil;
  control    = fn;
  dump       = nil;

  stopped = 0;

cycle:

  op_code[number_value(car(control))]();

  if (!stopped) goto cycle;

  return stack;
}

void init() {

  gc_init();

  true     = symbol("T");
  false    = symbol("F");
  nil      = symbol("NIL");

  stack    = nil;
  control  = nil;
  environ  = nil;
  dump     = nil;

  work     = nil;

  strings  = nil;
}

