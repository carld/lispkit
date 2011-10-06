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

#define MAX_TOKEN_LENGTH  80

/*
 * <s-exp> ::= <atom> | (<s-exp list>)
 * <s-exp list> ::= <s-exp> | <s-exp> . <s-exp> | <s-exp> <s-exp list>
 */

struct Token {
  FILE *fp;
  char * file;
  char * token;
  unsigned line;
  unsigned pos;
  unsigned word;
};

static char token_space[MAX_TOKEN_LENGTH];
static struct Token token;

static Object * s_exp(void);
static Object * s_exp_list(void);

enum { T_SYMBOL = 1, 
       T_NUMBER = 2, 
       T_DOT = 3, 
       T_LEFTPAREN = 4, 
       T_RIGHTPAREN = 5, 
       T_END };

void start_scan(FILE *fp) {
  token.fp   = fp;
  token.line = 0;
  token.pos  = 0;
  token.word = 0;
}

void scanner(void) {
  int ch, next_ch;
  char *ptr = token_space;

  token.token = NULL;
  bzero(token_space, MAX_TOKEN_LENGTH);

  /* skip white space */
  for( ; !feof(token.fp); token.pos++) {
    ch = fgetc(token.fp);
    if (!isspace(ch)) {
      ungetc(ch, token.fp);
      break;
    }
    if (ch == '\n') {
      token.line++;
      token.pos = 0;
    }
  }

  for ( ; !feof(token.fp); token.pos++) {
    ch = fgetc(token.fp);
    *ptr++ = (char)ch;
    *ptr = '\0';
    if (ch == '(' || ch == ')' || ch == '.') {
      break;
    }
    next_ch = fgetc(token.fp);
    ungetc(next_ch, token.fp);
    if (isspace(next_ch) || next_ch == '(' || next_ch == ')' || next_ch == '.') {
      break;
    }
  }

  if (strlen(token_space) > 0) {
    token.token = token_space;
  }
}

const char *token_type_str(int type) {
  switch(type) {
    case T_SYMBOL: return "Symbol";
    case T_NUMBER: return "Number";
    case T_DOT: return "Dot";
    case T_LEFTPAREN: return "Left parenthesis";
    case T_RIGHTPAREN: return "Right parenthesis";
    case T_END: return "End of file";
  }
  return "Unknown token type";
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
    printf("Error - did not expect token '%s'\n", token.token);
    printf("Expected token type: %s\n", token_type_str(type));
    printf("Line %d, column %d\n", token.line, token.pos);
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
      printf("Error - did not expect token '%s'\n", token.token);
      printf("Line %d, column %d\n", token.line, token.pos);
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
  start_scan(fp);
  scanner();
  exp = s_exp();
  return exp;
}

Object * get_exp_list(FILE *fp) {
  Object *exp;
  start_scan(fp);
  scanner();
  exp = s_exp_list();
  return exp;
}

