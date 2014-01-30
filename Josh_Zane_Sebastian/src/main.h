#define T_INT 0
#define T_CHR 1
#define T_RTN 2

struct stack_node {
    struct stack_node *cdr;
    union node_data data;
    char type;
}

union node_data {
    struct routine routine;
    int numval;
}

struct routine {
    struct routine *parent;
    struct iq_node *nodes;
    int num_nodes;
}

struct iq_node {
    struct iq_node *next;
    union node_data instr;
    char type;
}
