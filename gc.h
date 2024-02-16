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
#ifndef GC_H
#define GC_H 1

struct GCHeader {
  int size;
  int type;
  int marked;
  int id;
};

void gc_init(void);
Object * gc_alloc(void);
void gc_collect_garbage(void);
void gc_collect(void);
void gc_mark(Object *);
void gc_exit(void);
void gc_stats(void);

#define gc_header(o)  ((struct GCHeader *)o - 1)

#endif

