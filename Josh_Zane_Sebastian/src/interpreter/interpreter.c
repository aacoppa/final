#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void runStep(struct stack_node node, struct stack_node *top) {
  switch( node.data.type ) {
  case T_INT: push(node, top);
    break;
  case T_SBRTN:
    push(node, top);
    break;
  case T_char 1:
    switch( nonde.data.numval ) {
    case '+':
      int x = pop(top);
      int y = pop(top);
      push(x + y);
      break;
    case '_':
      push(0);
    case '-':
      int x = pop(top);
      int y = pop(top);
      push(x - y);
      break;
    case '*':
      int x = pop(top);
      int y = pop(top);
      push(x * y);
      break;
    case '/':
      int x = pop(top);
      int y = pop(top);
      push(x / y);
      break;
    case '=':
      int x = pop(top);
      int y = pop(top);
      if(x == y)
	push(-1);
      else
	push(0);
      break;
    case '<':
      int x = pop(top);
      int y = pop(top);
      if(x < y)
	push(-1);
      else
	push(0);
      break;
    case '~':
      int x = pop(top);
      int y = pop(top);
      if(x == y):
	push(0);
      else
	push(-1);
    }
  }
}

struct stack_node pop(struct stack_node *top) {
  struct stack_node pop = *top;
  *top = *(pop.cdr);
  return pop;
}

void push(struct stack_node node, struct stack_node *top) {
  node.cdr = top;
  *top = node;
}
