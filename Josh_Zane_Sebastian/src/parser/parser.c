#include <stdlib.h>
#include <stdio.h>
#include types.c
#include <string.h>

/* We can:
   Set something equal to something
   Function calls
   Function definitions
   Returning things -- delimiter '<'
   Printing things
   Iffing */

struct call parseCall(char* statement);
struct function parseFunction(char* statement);

char* fixSpacing(char* code) {
  char* fixedCode = malloc(sizeof(char) * (strlen(code) + 1));
  fixedCode = strcpy(fixedCode, code);
  char* doubleSpace = strstr(fixedCode, "  ");
  char* movingIndex;
  for( ; doubleSpace; doubleSpace = strstr(fixedCode, "  ")) {
    for(movingIndex = doubleSpace; movingIndex&; movingIndex++) {
      movingIndex[0] = movingIndex[1];
    }
  }
  return fixedCode;
}

char** spcTokenize(char* regCode) {
  int n = 0;
  int i;
  for(i = 0; regCode[i]; i++) {
    if(regCode[i] == ' ') {
      n++;
    }
  }
  char** spcTokens = malloc(sizeof(char*) * (n+1));
  int k;
  for(i = 0; i < n+1; i++) {
    k = strchr(regCode, ' ') - regCode;
    regCode[k] = NULL;
    spcTokens[i] = regCode + k + 1;
  }
}
