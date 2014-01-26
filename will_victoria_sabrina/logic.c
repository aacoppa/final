#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include <string.h>
// check if territories are adjacent
char tadjacent(territory *t1, territory *t2) {
  int *p = t1->neighbors;
  while(*p > -1) {
    if (strcmp(terrs[*p].name, t2->name) == 0)
      return 1;
    p++;
  }
  return 0;
}
