#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//TODO, queue a function's arguments, void enqueue()

void runStep(struct iq_node node, struct stack_node *top) {
  switch( node.data.type ) {
  case T_INT:
    struct stack_node topush;
    topush.data = node.data;
    topush.type = node.type;
    push(topush, top);
    break;

  case T_SBRTN:
    struct stack_node topush;
    topush.data = node.data;
    topush.type = node.type;
    push(node, top);
    break;


  case T_CHAR:
    switch( nonde.data.numval ) {
    case '+':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      x.data += y.data;
      push(x, top);
      break;

    case '_':
      struct stack_node x = pop(top);
      x.data = -(x.data);
      push(x, top);
      break;

    case '-':
      struct stack_node x = pop(top);
      struct stach_node y = pop(top);
      x.data -= y.data;
      push(x, top);
      break;

    case '*':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      x.data *= y.data;
      push(x, top);
      break;

    case '/':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      x.data /= y.data;
      push(x, top);
      break;

    case '=':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      if(x == y)
	x.data = -1;
      else
	x.data = 0;
      push(x, top);
      break;

    case '<':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      if(x < y)
	x.data = -1;
      else
	x.data = 0;
      push(x, top);
      break;

    case '&':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      x.data = -(x.data * y.data);
      break;

    case '~':
      struct stack_node x = pop(top);
      if(x.data)
	x.data = 0;
      else
	x.data = -1;
      push(x, top);
      break;

    case '|':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      if(x.data)
	push(x, top);
      else
	push(y, top);
      break;

    case '%':
      pop(top);
      break;
     
    case '\\':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      push(x, top);
      push(y, top);
      break;

    case '@':
      struct stack_node x = pop(top);
      struct stack_node y = pop(top);
      struct stack_node z = pop(top);
      push(y, top);
      push(x, top);
      push(z, top);
      break;

    case '$':
      struct stack_node n = pop(top);
      int i;
      struct stack_node *x = top;
      for(i = 0; i < n.data; i++)
	x = x->cdr;
      push(*x, top);
      break;

    case '?':
      struct stack_node b = pop(top);
      struct stack_node f = pop(top);
      if(b)
	queue_skip(f);
      break;

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
