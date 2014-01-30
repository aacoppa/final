#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.h"

int main(int argc, char **argv) {
    struct stack_node *stack_top = 0;
    struct routine cur_level;
    int fd = open(argv[1], O_RDONLY);
    char in[16], i = 0;
    char in_comment = 0;

    while (read(fd, in+i, 1)) {
        // ignore comments
        if (in_comment) {
            if (in[0] == '}')
                in_comment = 1;
            i = 0;
        } else if (in[i] == '{') {
            in_comment = 1;
        } else {
            if (i) {
                // previous text must have been a long number
                if (isdigit(in[0]))
                    i++;
                else {
                    // save the number to the queue
                    struct iq_node node;
                    node.type = T_INT;
                    union node_data data;
                    node.instr = data;
                    char c = in[i];
                    in[i] = 0;
                    data.numval = atoi(in);
                    add_node(node, cur_level);
                    i = 0;

                    // deal with the new character
                    if (c == '[') {
                        struct iq_node new_node;
                        new_node.type = T_RTN;
                        union node_data data;
                        new_node.instr = data;
                        struct routine subroutine;
                        data.routine = subroutine;

                        add_node(new_node, cur_level);
                        subroutine.parent = &cur_level;
                        cur_level = subroutine;
                    } else if (c == ']') {
                        cur_level = *cur_level.parent;
                    } else if (c == ' ' || c == '\t' || c == '\n') {
                        // ignore
                    } else {
                        struct iq_node new_node;
                        new_node.type = T_CHR;
                        union node_data data;
                        new_node.instr = data;
                        data.numval = c;
                    }
                    i = 0;
                }
            } else {
                if (isdigit(in[0]))
                    i++;
                else {
                    if (in[0] == '[') {
                        struct iq_node node;
                        node.type = T_RTN;
                        union node_data data;
                        node.instr = data;
                        struct routine subroutine;
                        data.routine = subroutine;

                        add_node(node, cur_level);
                        subroutine.parent = &cur_level;
                        cur_level = subroutine;
                    } else if (in[0] == ']') {
                        cur_level = *cur_level.parent;
                    } else if (in[0] == ' ' || in[0] == '\t' || in[0] == '\n') {
                        // ignore
                    } else {
                        struct iq_node node;
                        node.type = T_CHR;
                        union node_data data;
                        node.instr = data;
                        data.numval = in[0];
                    }
                    i = 0;
                }
            }
        }
    }
}

void add_node(struct iq_node node, struct routine routine) {
    if (routine.nodes) {
        struct iq_node cur_node = *routine.nodes;
        while (cur_node.next)
            cur_node = *cur_node.next;
        cur_node.next = &node;
    } else {
        routine.nodes = &node;
    }
}
