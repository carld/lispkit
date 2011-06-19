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
#include <ctype.h>
#include <stdlib.h>

#include "lispkit.h"

/*
 * <s-exp> ::= <atom> | (<s-exp list>)
 * <s-exp list> ::= <s-exp> | <s-exp> . <s-exp> | <s-exp> <s-exp list>
 */

extern struct Token token;

Object * s_exp(void);
Object * s_exp_list(void);

enum { T_SYMBOL = 1, T_NUMBER = 2, T_DOT = 3, T_LEFTPAREN = 4, T_RIGHTPAREN = 5, T_END };

char *type_str(int type) {
  switch(type) {
    case T_SYMBOL: return "symbol";
    case T_NUMBER: return "number";
    case T_DOT: return "dot";
    case T_LEFTPAREN: return "left paren";
    case T_RIGHTPAREN: return "right paren";
    case T_END: return "EOF";
  }
  return "unknown";
}

int token_type(void) {
  if (token.token == NULL) return T_END;
  switch(token.token[0]) {
    case '.': return T_DOT;
    case '(': return T_LEFTPAREN;
    case ')': return T_RIGHTPAREN;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': return T_NUMBER;
    default:  return T_SYMBOL;
  }
}

void match(int type) {
  if (type != token_type()) {
    printf("Error - expected %d %s , got '%s'\n",
        type, type_str(type), token.token);
    printf("Line %d, word %d\n", token.line, token.word);
    exit(-1);
  }
  scanner();
}

Object * s_exp(void) {
  Object *cell = _nil;
  switch(token_type()) {
    case T_NUMBER:
      cell = number(atoi(token.token));
      match(T_NUMBER);
      break;
    case T_SYMBOL:
      cell = symbol(token.token);
      match(T_SYMBOL);
      break;
    case T_LEFTPAREN:
      match(T_LEFTPAREN);
      cell = s_exp_list();
      match(T_RIGHTPAREN);
      break;
    case T_END:
      break;
    default:
      printf("%s:%d error, did not expect type %d %s\n",
          __FILE__, __LINE__, token_type(), type_str(token_type()));
      exit(-1);
  }
  return cell;
}

Object * s_exp_list(void) {
  Object *cell = _nil;

  cell = cons(s_exp(), _nil);

  switch(token_type()) {
    case T_RIGHTPAREN:
      break;
    case T_DOT:
      match(T_DOT);
      cell->Cons.cdr = s_exp();
      break;
    case T_END:
      break;
    default:
      cell->Cons.cdr = s_exp_list();
      break;
  }

  return cell;
}

Object * get_exp(FILE *fp) {
  Object *exp;
  tokenize(fp);
  scanner();
  exp = s_exp();
  tokenizer_free();
  return exp;
}

Object * get_exp_list(FILE *fp) {
  Object *exp;
  tokenize(fp);
  scanner();
  exp = s_exp_list();
  tokenizer_free();
  return exp;
}

