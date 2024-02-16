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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

typedef void (*OpCode) (void);

Object *_stack;
Object *_environ;
Object *_control;
Object *_dump;

Object *_true;
Object *_false;
Object *_nil;

Object *_work;

static int stopped = 0;


void _ldc(void) {
  _stack   = cons(car(cdr(_control)), _stack);
  _control = cdr(cdr(_control));
}

void _ld(void) {
  int i = 0;
  _work    = _environ;

  for (i = 1; i <= number_value(car(car(cdr(_control)))); ++i) {
    _work = cdr(_work);
  }
  _work    = car(_work);
  for (i = 1; i <= number_value(cdr(car(cdr(_control)))); ++i) {
    _work = cdr(_work);
  }
  _work    = car(_work);
  _stack   = cons(_work, _stack);
  _control = cdr(cdr(_control));
  _work    = _nil;
}

void _car(void) {
  _stack   = cons(car(car(_stack)), cdr(_stack));
  _control = cdr(_control);
}

void _cdr(void) {
  _stack   = cons(cdr(car(_stack)), cdr(_stack));
  _control = cdr(_control);
}

void _atom(void) {
  if (is_number(car(_stack)) || is_symbol(car(_stack))) {
    _stack = cons(_true, cdr(_stack));
  } else {
    _stack = cons(_false, cdr(_stack));
  }
  _control = cdr(_control);
}

void _cons(void) {
  _stack   = cons(cons(car(_stack),car(cdr(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _sub(void) {
  _stack   = cons(number(number_value(car(cdr(_stack))) - number_value(car(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _add(void) {
  _stack   = cons(number(number_value(car(cdr(_stack))) + number_value(car(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _mul(void) {
  _stack   = cons(number(number_value(car(cdr(_stack))) * number_value(car(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _div(void) {
  _stack   = cons(number(number_value(car(cdr(_stack))) / number_value(car(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _rem(void) {
  _stack   = cons(number(number_value(car(cdr(_stack))) % number_value(car(_stack))),cdr(cdr(_stack)));
  _control = cdr(_control);
}

void _leq(void) {
  if (number_value(car(cdr(_stack))) <= number_value(car(_stack))) {
    _stack = cons(_true, cdr(cdr(_stack)));
  } else {
    _stack = cons(_false, cdr(cdr(_stack)));
  }
  _control = cdr(_control);
}

void _eq(void) {
  if ((is_symbol(car(_stack)) && is_symbol(car(cdr(_stack))) && (strcmp(string_value(car(_stack)), string_value(car(cdr(_stack)))) == 0)) ||
    (is_number(car(_stack)) && is_number(car(cdr(_stack))) && number_value(car(_stack)) == number_value(car(cdr(_stack))))) {
    _stack = cons(_true, cdr(cdr(_stack)));
  } else {
    _stack = cons(_false, cdr(cdr(_stack)));
  }
  _control = cdr(_control);
}

void _ldf(void) {
  _work = cons(car(cdr(_control)), _environ);
  _stack = cons(_work, _stack);
  _control = cdr(cdr(_control));
  _work = _nil;
}

void _rtn(void) {
  _stack   = cons(car(_stack), car(_dump));
  _environ = car(cdr(_dump));
  _control = car(cdr(cdr(_dump)));
  _dump    = cdr(cdr(cdr(_dump)));
}

void _dum(void) {
  _environ = cons(_nil, _environ);
  _control = cdr(_control);
}

void _rap(void) {
  _dump         = cons(cdr(_control),_dump);
  _dump         = cons(cdr(_environ), _dump);
  _dump         = cons(cdr(cdr(_stack)), _dump);
  _environ      = cdr(car(_stack));
  _environ->Cons.car = car(cdr(_stack));
  _control      = car(car(_stack));
  _stack        = _nil;
}

void _sel(void) {
  _dump = cons(cdr(cdr(cdr(_control))),_dump);
  if (strcmp(string_value(car(_stack)), string_value(_true)) == 0) {
    _control = car(cdr(_control));
  } else {
    _control = car(cdr(cdr(_control)));
  }
  _stack   = cdr(_stack);
}

void _join(void) {
  _control = car(_dump);
  _dump    = cdr(_dump);
}

void _ap(void) {
  _dump    = cons(cdr(_control), _dump);
  _dump    = cons(_environ, _dump);
  _dump    = cons(cdr(cdr(_stack)), _dump);
  _environ = cons(car(cdr(_stack)),cdr(car(_stack)));
  _control = car(car(_stack));
  _stack   = _nil;
}

void _stop(void) {
  stopped = 1;
}

OpCode op_code[] = {
  0,
  _ld,
  _ldc,
  _ldf,
  _ap,
  _rtn,
  _dum,
  _rap,
  _sel,
  _join,
  _car,
  _cdr,
  _atom,
  _cons,
  _eq,
  _add,
  _sub,
  _mul,
  _div,
  _rem,
  _leq,
  _stop
};

Object * execute(Object *fn, Object *args) {

  _stack      = cons(args, _nil);
  _environ    = _nil;
  _control    = fn;
  _dump       = _nil;

  stopped = 0;

  for ( ; stopped == 0; ) {
    op_code[number_value(car(_control))]();
  }

  return _stack;
}

void init(void) {
  gc_init();

  _true     = symbol("T");
  _false    = symbol("F");
  _nil      = symbol("NIL");

  _stack    = _nil;
  _control  = _nil;
  _environ  = _nil;
  _dump     = _nil;

  _work     = _nil;
}

