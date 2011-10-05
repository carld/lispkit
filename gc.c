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
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

#define NUM_CELLS    16384

unsigned        alloc_counter;
unsigned        collect_counter;
unsigned        num_cells;

void           *mem;
Object        **cells;
Object         *ff;

void 
gc_mark(Object * object)
{
  if (gc_header(object)->marked == 0) {
    gc_header(object)->marked = 1;
    if (is_atom(object) == 0) {
      gc_mark(car(object));
      gc_mark(cdr(object));
    }
  }
}

void 
gc_init()
{
  const unsigned  cell_size = sizeof(struct GCHeader) + sizeof(Object);
  unsigned char  *ptr;
  int             i;

  num_cells = getenv("LISPKIT_MEMORY") ?
    atoi(getenv("LISPKIT_MEMORY")) : NUM_CELLS;

  mem = calloc(num_cells, cell_size);
  cells = calloc(num_cells, sizeof(Object *));

  alloc_counter = 0;
  collect_counter = 0;

  ff = NULL;

  for (i = 0, ptr = mem; i < num_cells; i++, ptr += cell_size) {
    cells[i] = (Object *) ((struct GCHeader *) ptr + 1);
    cells[i]->Cons.cdr = ff;
    ff = cells[i];
  }
}

void 
gc_exit()
{
  free(mem);
  free(cells);
}

Object         *
gc_alloc()
{
  Object         *object;
  static unsigned _id = 0;

  if (ff == NULL) {
    gc_collect_garbage();
  }
  object = ff;
  ff = ff->Cons.cdr;
  gc_header(object)->type = 0;
  gc_header(object)->id = _id;

  alloc_counter++;

  return object;
}

void 
gc_collect()
{
  int             i;
  for (i = 0; i < NUM_CELLS; i++) {
    if (gc_header(cells[i])->marked == 0) {
      cells[i]->Cons.cdr = ff;
      ff = cells[i];

      collect_counter++;
    }
  }
}

void 
gc_collect_garbage()
{
  int             i;
  for (i = 0; i < NUM_CELLS; i++) {
    gc_header(cells[i])->marked = 0;
  }
  gc_mark(_stack);
  gc_mark(_environ);
  gc_mark(_control);
  gc_mark(_dump);
  gc_mark(_work);
  gc_mark(_true);
  gc_mark(_false);
  gc_mark(_nil);

  gc_collect();

  if (ff == NULL) {
    printf("out of memory: %s", __func__);
    exit(-1);
  }
}

void 
gc_stats()
{
  printf("Cells:     %u\n", num_cells);
  printf("Allocates: %u\n", alloc_counter);
  printf("Collects:  %u\n", collect_counter);
}

