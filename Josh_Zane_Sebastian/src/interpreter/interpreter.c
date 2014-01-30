#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void runStep(struct stack_node node, struct stack_node *top) {
  switch( node.data.type ) {
  case 0:
    struct stack_node push;
    push.cdr = top;
    push.data = node.data;
    *top = push;
    break;
  }
}
