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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lispkit.h"
#include "gc.h"

const char help[] = "  lispkit [compiler] [file|--] \n";

int main(int argc, char *argv[]) {
  int arg = 1;

  FILE * fp_fn   = NULL;
  FILE * fp_args = stdin;

  Object *fn, *args, *result;

  if (argc < 2) {
    puts(help); 
    return(-1);
  }

  if (arg < argc) {
    fp_fn = fopen(argv[arg], "ra");
    if (fp_fn == NULL) {
      printf("Could not load '%s'\n", argv[arg]);
      return(-1);
    }
    arg++;
  }

  if (arg < argc) {
    fp_args = fopen(argv[arg], "ra");
    if (fp_args == NULL) {
      printf("Could not load '%s'\n", argv[arg]);
      return(-1);
    }
  }


  init();

  fn = _nil; args = _nil; result = _nil;

  fn   = get_exp(fp_fn);
  args = get_exp_list(fp_args);

  result = execute(fn, args);

  print(result); printf("\n");

  fclose(fp_fn);
  fclose(fp_args);

  gc_exit();
  intern_free();

  return 0;
}


