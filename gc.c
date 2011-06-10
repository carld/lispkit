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
#include <assert.h>

#include "lispkit.h"
#include "gc.h"

#define cells      262144
#define cell_size  sizeof(struct GCHeader) + 32
#define mem_size   cells * cell_size

unsigned char mem[mem_size];

static unsigned char *next_cell = mem;

void gc_init() {
  memset(mem, 0, mem_size);
}

void * gc_alloc(int size) {
  struct GCHeader *gc_object = 0;
  static unsigned _id = 0;

  assert(size < 256); 

  gc_object = (struct GCHeader *) next_cell;
  gc_object->id = _id++;

  next_cell += cell_size;

  if (next_cell > (mem + mem_size)) {
    fprintf(stderr, "out of memory, %s:%d (id = %u)\n", __FILE__, __LINE__, _id);
    exit(-1);
  }

  gc_object->size = size;
  return (void *)(gc_object + 1);
}

void gc_collect() {

}

#if 0
void gc_graph_objects(FILE *fp, Object *obj) {
    switch (gc_object->type) {
      case CONS:    fprintf(fp, "cell%d [label=\"<f0> car|<f1> cdr\"];", gc_object->id); break;
      case SYMBOL:  fprintf(fp, "cell%d,[label=\"<f0> %s\"];", gc_object->id, object->Symbol.symbol); break;
      case NUMBER:  fprintf(fp, "cell%d,[label=\"<f0> %ld\"];", gc_object->id, object->Number.number); break;
    }
}
#endif

void gc_graph(FILE *fp, Object *_obj) {
#if 0
  int cell;

  printf("Graphing....\n");

  fprintf(fp, 
      "digraph gc {\n"
      "  node [shape=record]\n"
    );

  for(cell = 0; cell < cells; cell++) {
    struct GCHeader *gc_object = 0;
    Object *object      = 0;
    gc_object = (struct GCHeader *) mem + (cell * (sizeof (struct GCHeader) + 256));
    object = (Object *) (gc_object + 1);
    if (gc_object->type == CONS) {
      fprintf(fp, "cell%d:f0 -> cell%d:f0\n", gc_object->id, gc_header(object->Cons.car)->id);
      fprintf(fp, "cell%d:f0 -> cell%d:f0\n", gc_object->id, gc_header(object->Cons.cdr)->id);
    }
  }

  fprintf(fp, 
      "}\n"
    );
  printf("Graph done!\n");
#endif
}

