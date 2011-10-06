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
#include <stdlib.h>
#include <string.h>

#include "lispkit.h"
#include "gc.h"

int is_null(Object *obj) 
{
  return /* (obj == _nil) || */
      (	(gc_header(obj)->type == SYMBOL) 
     	 && (strcmp(string_value(obj), string_value(_nil)) == 0) );
}

void 
print(Object * obj)
{
  if (!obj || obj == _nil )
    return;

  if (gc_header(obj)->type == NUMBER) {
    printf("%ld ", obj->Number.number);
  } else if (gc_header(obj)->type == SYMBOL) {
    printf("%s ", obj->Symbol.symbol);
  } else if (gc_header(obj)->type == CONS) {
    if (gc_header(car(obj))->type != CONS 
        && gc_header(cdr(obj))->type != CONS 
        && !is_null(cdr(obj))
	) {
      print(car(obj));
      printf(". ");
      print(cdr(obj));
    } else {
      if (gc_header(car(obj))->type == CONS)
	printf("( ");
      print(car(obj));
      if (gc_header(car(obj))->type == CONS)
	printf(") ");

      if (!is_null(cdr(obj))) {
     	print(cdr(obj));
      }
    }
  } else {
    printf("Unknown type: %s\n", type_str(gc_header(obj)->type));
    exit(-1);
  }
}

