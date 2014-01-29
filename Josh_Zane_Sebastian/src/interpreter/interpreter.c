#include <stdlib.h>
#include <stdio.h>

// interprets a parsed version of the program, split into statements
void interpret(struct program* program) {
  int i;

  for (i = 0; i < program->length; i++) {
    struct statement stmt = program->statements[i];
    if (stmt.type == ST_ASSGN) {
      assign(stmt.parts[1], stmt.parts[0]);
    } else if (stmt.type == ST_RETRN) {
      retVal(stmt.parts[0]);
    } else if (stmt.type == ST_FUNCT) {
      runFunc(stmt.parts[0]);
    }
  }
}

// runs a function
void runFunc(struct fnCall* fnCall) {
  int i;

  for (i = 0; i < fnCall->numArgs; i++) {
    if (!isAtomic(fnCall->args[i])) {
      setArg(fnCall, i, future(fnCall, i));
    }
  }

  for (i = 0; i < fnCall->numArgs; i++) {
    while(!(fnCall->args[i].fulfilled));
  }

  // run the function somehow
}
