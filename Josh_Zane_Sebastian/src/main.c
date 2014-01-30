#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char **argv) {
    struct stack_node *stack_top = 0;
    struct routine cur_level;
    int fd = fopen(argv[1]);
    int sbrtn_depth = 0;
    char in[16], i = 0;

    while (read(fd, in+i, 1)) {
        if (i) {
            // previous text must have been a long number
            if (!isdigit(in[i])) {
                in[i+1] = in[i];
                in[i] = 0;
                // add the int value to the instruction queue
                struct iq_node new_node;
                union node_data data;
                data.numval = atoi(in);
                new_node.instr = data;
                new_node.type = T_INT;

                in[0] = in[i+1];
                i = 1;
                // ignore whitespace
                if (in[0] == ' ' || in[0] == '\t' || in[0] == '\n') {
                    i--;
                    continue;
                }
                if (in[0] == '[') {
                    sbrtn_depth++;
                    continue;
                }
            }
        } else {

        }
    }
}
