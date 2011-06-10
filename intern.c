#include <stdlib.h>
#include <string.h>

const char * intern_string(char *string) {
  static const char * pstringz[32768] = { NULL };
  const char ** ptr;

  for (ptr = pstringz; *ptr != NULL; ptr++) {
    if (strcmp(*ptr, string) == 0) {
      return *ptr;
    }
  }

  *ptr = strdup(string);

  return *ptr;
}

