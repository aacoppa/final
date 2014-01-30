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
  char* fixedCode = calloc(sizeof(char), strlen(code) + 1);
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

struct statement* parse(char* code) {
  char* regCode = fixSpacing(code);
  int n = 0;
  int i;
  for(i = 0; regCode[i]; i++) {
    if(regCode[i] == ' ') {
      n++;
    }
  }
  char** spcTokens = calloc(sizeof(char*), n+1);
