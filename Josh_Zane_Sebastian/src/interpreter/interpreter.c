#include <stdlib.h>
#include <stdio.h>
#include "dictionary.c"

// interprets a parsed version of the program, split into statements
void interpret(struct program* program) {
  int i;

  for (i = 0; i < program->length; i++) {
    struct statement stmt = program->statements[i];
    if (stmt.type == ST_ASSGN) {
      // a = | b c
      assign(stmt.parts[1], stmt.parts[0]);
    } else if (stmt.type == ST_RETRN) {
      // < $| b c
      retVal(stmt.parts[0]);
    } else if (stmt.type == ST_FUNCT) {
      // | b c
      runFunc(stmt.parts[0]);
    }
  }
}

// runs a function
void runFunc(struct fnCall* fnCall) {
  int i;

  // create futures where necessary
  for (i = 0; i < fnCall->numArgs; i++) {
    if (!isAtomic(fnCall->args[i])) {
      setArg(fnCall, i, future(fnCall, i));
    }
  }

  // wait for the futures to be evaluated
  for (i = 0; i < fnCall->numArgs; i++) {
    while(!(fnCall->args[i].fulfilled)) {
      usleep(100 * 1000); // sleep for 0.1s
    }
  }

  // if the function is a builtin, run that builtin
  if (isBuiltin(fnCall)) {
    runBuiltin(fnCall);
  } else {
    // TODO run the function somehow
  }
}

// tests to see if a function is one of the builtins
int isBuiltin(struct fnCall* fnCall) {
  char *name = fnCall->name;

  if (!strcmp(name, "!") || !strcmp(name, "|")) {
    return 1;
  }

  if (!strcmp(name, "[") || !strcmp(name, "]")) {
    return 1;
  }

  if (!strcmp(name, ">") || !strcmp(name, "?")) {
    return 1;
  }
}
