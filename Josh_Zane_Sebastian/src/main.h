struct stack_node {
    struct stack_node *cdr;
    int data;
    char type;
}

struct subroutine {
    struct stack_node *nodes;
    int num_nodes;
}
