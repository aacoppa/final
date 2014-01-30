#define T_INT 0
#define T_CHAR 1
#define T_SBRTN 2

struct stack_node {
    struct stack_node *cdr;
    union node_data data;
}

union node_data {
    struct subroutine srtine;
    int numval;
    char type;
}

struct subroutine {
    struct stack_node *nodes;
    int num_nodes;
}
