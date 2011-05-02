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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

#define issyntax(c) (c == '(' || c == ')' || c == '.')

Object * scan(FILE *fp) {
  Object * obj = nil;
  char     buffer[80];
  char *   ptr = buffer;
  int      ch  = EOF;
  *ptr = '\0';
  ch = getc(fp);
  while(isspace(ch)) {
    ch = getc(fp);
  }
  while(!isspace(ch) && !feof(fp)) {
    *ptr++ = ch;
    ch = getc(fp);
    if ((isalnum(buffer[0]) && issyntax(ch)) || (issyntax(buffer[0]) && isalnum(ch))) {
      fseeko(fp, -1, SEEK_CUR);
      break;
    }
  }
  *ptr = '\0';
  if (strlen(buffer) == 0) goto done;
  obj = symbol(buffer);
done:
  return obj;
}

void push(Object **stack, Object *obj) {
  if (*stack == nil)
    *stack = cons(obj, nil);
  else
    *stack = cons(obj, *stack);
}

Object *pop(Object **stack) {
  Object *obj = nil;
  if (*stack == nil) return nil;
  obj         = car(*stack);
  *stack      = cdr(*stack);
  return obj;
}

Object * read_expr(FILE *fp) {
  Object * stack1 = nil;
  Object * stack2 = nil;
  Object * sexpr  = nil;
  Object * obj    = nil;
  int ch = EOF;
  while(!feof(fp)) {
    obj = scan(fp); if (obj == nil) break;
    push(&stack1, obj);
  }
  while((obj = pop(&stack1)) != nil) {
    ch = string_value(obj)[0];
    if (isalpha(ch)) {
      sexpr = cons(obj, sexpr);
    } else if (isdigit(ch)) {
      obj = number(atoi(string_value(obj)));
      sexpr = cons(obj, sexpr);
    } else if (ch == '(') {
      sexpr = cons(sexpr, pop(&stack2));
    } else if (ch == ')') {
      push(&stack2, sexpr);
      sexpr = nil;
    } else if (ch == '.') {
      obj = pop(&stack1);
      ch = string_value(obj)[0];
      if (isdigit(ch))
        obj = number(atoi(string_value(obj)));
      sexpr = cons(obj, car(sexpr));
    }
  }
  return sexpr;
}


