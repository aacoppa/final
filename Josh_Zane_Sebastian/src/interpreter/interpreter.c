#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../main.h"

struct stack_node pop(struct stack_node*);
void push(struct stack_node, struct stack_node*);

//TODO, queue a function's arguments, void enqueue()

void runStep(struct iq_node node, struct stack_node *top) {
  struct stack_node topush;
  struct stack_node x, y, z, n, *w, b, f;
  int i;
  switch( node.type ) {
  case T_INT:
    topush.data = node.data;
    topush.type = node.type;
    push(topush, top);
    break;

  case T_RTN:
    topush.data = node.data;
    topush.type = node.type;
    push(node, top);
    break;


  case T_CHR:
    switch( node.data.numval ) {
    case '+':
      x = pop(top);
      y = pop(top);
      x.data.numval += y.data.numval;
      push(x, top);
      break;

    case '_':
      x = pop(top);
      x.data.numval *= -1;
      push(x, top);
      break;

    case '-':
      x = pop(top);
      y = pop(top);
      x.data.numval -= y.data.numval;
      push(x, top);
      break;

    case '*':
      x = pop(top);
      y = pop(top);
      x.data.numval *= y.data.numval;
      push(x, top);
      break;

    case '/':
      x = pop(top);
      y = pop(top);
      x.data.numval /= y.data.numval;
      push(x, top);
      break;

    case '=':
      x = pop(top);
      y = pop(top);
      if(x.data.numval == y.data.numval)
	      x.data.numval = -1;
      else
	      x.data.numval = 0;
      push(x, top);
      break;

    case '<':
      x = pop(top);
      y = pop(top);
      if(x.data.numval < y.data.numval)
	      x.data.numval = -1;
      else
	      x.data.numval = 0;
      push(x, top);
      break;

    case '&':
      x = pop(top);
      y = pop(top);
      x.data.numval = -(x.data.numval * y.data.numval);
      break;

    case '~':
      x = pop(top);
      if(x.data.numval)
	      x.data.numval = 0;
      else
	      x.data.numval = -1;
      push(x, top);
      break;

    case '|':
      x = pop(top);
      y = pop(top);
      if(x.data.numval)
	      push(x, top);
      else
	      push(y, top);
      break;

    case '%':
      pop(top);
      break;
     
    case '\\':
      x = pop(top);
      y = pop(top);
      push(x, top);
      push(y, top);
      break;

    case '@':
      x = pop(top);
      y = pop(top);
      z = pop(top);
      push(y, top);
      push(x, top);
      push(z, top);
      break;

    case '$':
      n = pop(top);
      w = top;
      for(i = 0; i < n.data.numval; i++)
	      w = w->cdr;
      push(*w, top);
      break;

    case '?':
      b = pop(top);
      f = pop(top);
      if(b.data.numval)
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
