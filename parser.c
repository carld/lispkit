#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "lispkit.h"

/*
 * <s-exp> ::= <atom> | (<s-exp list>)
 * <s-exp list> ::= <s-exp> | <s-exp> . <s-exp> | <s-exp> <s-exp list>
 */

extern void scanner(void);
extern struct Token token;

enum { T_SYMBOL = 1, T_NUMBER = 2, T_DOT = 3, T_LEFTPAREN = 4, T_RIGHTPAREN = 5, T_END };

char *type_str(int type) {
  switch(type) {
    case T_SYMBOL: return "symbol";
    case T_NUMBER: return "number";
    case T_DOT: return "dot";
    case T_LEFTPAREN: return "left paren";
    case T_RIGHTPAREN: return "right paren";
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
        token_type(), type_str(token_type()), token.token);
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

