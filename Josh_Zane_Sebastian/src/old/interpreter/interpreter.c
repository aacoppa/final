#include <stdlib.h>
#include <stdio.h>

#define ST_ASSGN 0
#define ST_FUNCT 1

// interprets a parsed version of the program, split into statements
void interpret(struct statement* statements, int numStatements) {
  int i;

  for (i = 0; i < numStatements; i++) {
    struct statement stmt = statements[i];
    if (stmt.type == ST_ASSGN) {
      // a = | b c
      assign(stmt.parts[1], stmt.parts[0]);
    } else if (stmt.type == ST_FUNCT) {
      // | b c
      runFunc(stmt.parts[0]);
    }
  }
}

void assign(struct statement, struct symbol) {
  
}

// runs a function
void runFunc(struct call *fnCall) {
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
int isBuiltin(struct call *fnCall) {
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

  if (!strcmp(name, "Y") || !strcmp(name, "<")) {
    return 1;
  }

  return 0;
}

struct variable runBuiltin(struct call *fnCall) {
  char *name = fnCall->name;
  struct variable result;

  if (strcmp(name, "!") == 0) {
    // NOT
    struct symbol thingToNegate = fnCall->args[0];
    char bit = thingToNegate.referant->val.bit;
    result.val.bit = !bit;
    result.typeid = 0; // bit type
  } else if (strcmp(name, "|") == 0) {
    // OR
    char a = fnCall->args[0].referant->val.bit;
    char b = fnCall->args[1].referant->val.bit;
    result.val.bit = a || b;
    result.typeid = 0; // bit type
  } else if (strcmp(name, "<")) {
    // RETURN
    struct variable arg = fnCall->args[0].referant;
    result.val.bit = arg.val.bit;
    result.val.typeid = arg.val.typeid;
    retVal(arg);
  } else if (strcmp(name, "Y")) {
    // IF
    char bit = fnCall->args[0].referant->val.bit;
    struct function f;
    struct llnode argList;
    struct call call;

    // check which func to run
    if (bit) {
      call.func = fnCall->args[1].referant->val.func;
      call.name = fnCall->args[1].name;
      argList = fnCall->args[2].referant->val.list;
    } else {
      call.func = fnCall->args[3].referant->val.func;
      call.name = fnCall->args[3].name;
      argList = fnCall->args[4].referant->val.list;
    }

    // count args
    int numArgs = 0;
    while (argList.car) {
      argList = *argList.cdr;
      numArgs++;
    }
    struct symbol* args = calloc(numArgs, sizeof(struct symbol));
    call.arguments = args;
    call.numargs = numArgs;
    runFunc(call);
    free(args);
  } else if (strcmp(name, "[") == 0) {
    // CAR
    struct llnode list = fnCall->args[0].referant->val.list;
    result = *list.car;
  } else if (strcmp(name, "]") == 0) {
    // CDR
    struct llnode list = fnCall->args[0].referant->val.list;
    result = *list.cdr;
  } else if (strcmp(name, "?") == 0) {
    // IS_EMPTY
    struct llnode list = fnCall->args[0].referant->val.list;
    if (list.car) {
      result.val.bit = 0;
    } else {
      result.val.bit = 1;
    }
    result.typeid = 0;
  }

  return result;
}
