#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "lispkit.h"

char *token_stack[65535];
int cur_tok;

unsigned linen, wordn;

struct Token token;

void scanner(void) {
  extern struct Token token;
  token.token = token_stack[cur_tok++];
  token.line  = linen;
  token.word  = wordn;
}

#if 0
int main (int argc, char *argv[], char *envp[]) {
#endif

void tokenize (FILE *fp) {
  char line[255];
  int stacki = 0;
  int i;

  cur_tok = 0;
  for (i = 0; i < 65535; i++) {
    token_stack[i] = NULL;
  }
  linen = 0;
  while( fgets(line, 255, fp) ) {
    const char sep[] = " \n\r\t"; 
    char *tok;
    wordn = 1;
    linen++;
    for (tok=strtok(line, sep); tok; tok=strtok(NULL,sep)) {
      token_stack[stacki++] = strdup(tok);
      wordn++;
    }
  }

#if 0
  for(i = 0; i < stacki; i++) {
    printf("token %d is '%s'\n", i, token_stack[i]);
  }
#endif

  if (ferror(fp) != 0) {
    printf("error: %s\n", strerror(errno));
  }
}

Object * get_exp(FILE *fp) {
  Object *exp;
  tokenize(fp);
  scanner();
  exp = s_exp();
  return exp;
}

Object * get_exp_list(FILE *fp) {
  Object *exp;
  tokenize(fp);
  scanner();
  exp = s_exp_list();
  return exp;
}

