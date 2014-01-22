#include <stdlib.h>
#include <stdio.h>

struct llnode;
struct arg;
struct function;
struct type;

struct llnode {
  struct type* car;
  struct llnode* cdr;
};

struct arg {
  char* name;
  struct type* arg;
};


struct function {
  struct arg* args;
  char* definition;
};

union value {
  char bit;
  struct llnode list;
};

struct type {
  union value val;
  char typeid;
};
