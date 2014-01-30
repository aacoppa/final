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

void fixSpacing(char* code) {
  char* fixedCode = calloc(sizeof(char), strlen(code) + 1);
  fixedCode = strcpy(fixedCode, code);
  int n = strlen(fixedCode);
  char* doubleSpace = strstr(fixedCode, "  ");
  char* movingIndex;
  for( ; doubleSpace; doubleSpace = strstr(fixedCode, "  ")) {
    for(movingIndex = doubleSpace; movingIndex&; movingIndex++) {
      movingIndex[0] = movingIndex[1];
    }
  }
  return fixedCode;
}
