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
#include <string.h>
#include <errno.h>

#include "lispkit.h"
#include "gc.h"

const char      help[] = "  lispkit [file] [ile|--] \n";
int verbose = 0;

int
main(int argc, char *argv[], char *envp[])
{
  int             arg = 1;
  FILE           *fp[2];
  int             fpi = 0;
  Object         *fn, *args, *result;

  if (argc < 2) {
    puts(help);
    exit(-1);
  }

  for (arg = 1; arg < argc; arg++) {
    if (*(argv[arg]) == '-') {
      if (*(argv[arg]+1) == 'v') {
        verbose++;
      }
    } else {
      fp[fpi] = fopen(argv[arg], "ra");
      if (fp[fpi] == NULL || ferror(fp[fpi]) != 0) {
        printf("Could not load '%s'\n", argv[arg]);
        printf("%s\n", strerror(errno));
        exit(-1);
      }
      fpi++;
    }
  }

  init();

  fn     = get_exp(fp[0]);
  args   = get_exp_list(fp[1]);
  result = execute(fn, args);

  print(result);
  printf("\n");

  fclose(fp[0]);
  fclose(fp[1]);

  if (verbose) {
    gc_stats();
  }

  gc_exit();
  intern_free();

  return 0;
}

