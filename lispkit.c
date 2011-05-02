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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

Object * object_alloc() {
  return gc_alloc(sizeof(Object));
}

const char * intern_string(char *string) {
  char * buffer = 0;
  Object * obj = nil;

  for (obj = strings; obj != nil; obj = cdr(obj)) {
    if (strcmp(string, string_value(car(obj))) == 0) {
      return string_value(car(obj));
    }
  }

  buffer = gc_alloc(strlen(string)+1);
  strcpy(buffer, string);

  obj    = object_alloc();
  gc_header(obj)->type = STRING;
  obj->String.string   = buffer;

  strings = cons(obj, strings);

  return string_value(car(strings));
}

Object * cons(Object *_car, Object *_cdr)
{
  Object *obj = object_alloc();
  gc_header(obj)->type = CONS;
  obj->Cons.car = _car;
  obj->Cons.cdr = _cdr;
  return obj;
}

Object * car(Object *obj) {
  if (gc_header(obj)->type != CONS) {
    printf("Not a cons object (car)\n");
    print(obj);
    exit(-1);
  }
  return obj->Cons.car;
}

Object * cdr(Object *obj) {
  if (gc_header(obj)->type != CONS) {
    printf("Not a cons object (cdr)\n");
    print(obj);
    exit(-1);
  }
  return obj->Cons.cdr;
}

Object * number(int value) {
  Object *obj = object_alloc();
  gc_header(obj)->type = NUMBER;
  obj->Number.number = value;
  return obj;
}

Object * symbol(char *value) {
  Object *obj = object_alloc();
  gc_header(obj)->type = SYMBOL;
  obj->Symbol.symbol = intern_string(value);
  return obj;
}

int is_symbol(Object *obj) {
  return gc_header(obj)->type == SYMBOL;
}

int is_number(Object *obj) {
  return gc_header(obj)->type == NUMBER;
}

int is_atom(Object *obj) {
  return is_symbol(obj) || is_number(obj);
}

int is_cons(Object *obj) {
  return gc_header(obj)->type == CONS;
}

const char * string_value(Object *obj) {
  switch (gc_header(obj)->type) {
    case STRING: return obj->String.string;
    case SYMBOL: return obj->Symbol.symbol;
  }
  exit(-1);
  return NULL;
}

int number_value(Object *obj) {
  return obj->Number.number;
}


