#include <stdlib.h>
#include <stdio.h>

struct llnode;
struct symbol;
struct function;
union value;
struct variable;


struct llnode {
  struct type* car;
  struct llnode* cdr;
};

struct symbol {//I guess this is analogous to a symbol in lisp.
  char* name; //It has a name, which we use to keep track of it.
  struct variable* referant; //And an actual value in memory.
};

struct function {
  struct symbol* args; //these are the symbols that the function takes as arguments
  char* definition; // this is the function's actual definition, left in text form until parsing.
};

union value { //we have three primitive types, so every variable's value is one of those types
  char bit;
  struct llnode list;
  struct function func;
};

struct variable { //this is essentially an in-program variable
  union value val; //it has a value, i.e. what it equals
  char typeid; //and a type id that allows us to correctly extract the value from the union value.
};
