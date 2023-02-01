/* Maths functions for dealing with algebraic expressions */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Structures.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// https://stackoverflow.com/a/41394338
char *double_to_string(long double x) {
    int len = snprintf(NULL, 0, "%Lf", x);
    char *result = (char *)malloc(len + 1);
    snprintf(result, len + 1, "%Lf", x);

    return result;
}

/******************************************\
* `args` ARE IN A REVERSED ORDER TO NORMAL *
*                                          *
* ie., '9 / 2' will be passed as {2, 9}    *
\******************************************/

// Basic operators
// arg_n = 2
T_Tree_Node N_add(T_Tree_Node *args, unsigned int arg_n) {
    long double x = args[1].token.data + args[0].token.data;

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

// arg_n = 2
T_Tree_Node N_sub(T_Tree_Node *args, unsigned int arg_n) {
    long double x = args[1].token.data - args[0].token.data;

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

// arg_n = 2
T_Tree_Node N_times(T_Tree_Node *args, unsigned int arg_n) {
    long double x = args[1].token.data * args[0].token.data;

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

// arg_n = 2
T_Tree_Node N_div(T_Tree_Node *args, unsigned int arg_n) {
    long double x = args[1].token.data / args[0].token.data;

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

// arg_n = 2
T_Tree_Node N_pow(T_Tree_Node *args, unsigned int arg_n) {
    long double x = pow(args[1].token.data, args[0].token.data);

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

T_Tree_Node N_percent(T_Tree_Node *args, unsigned int arg_n) {
    long double x = args[0].token.data / 100;

    return new_tree_node((T_Token){T_NUM, double_to_string(x), x});
}

// repr -> function
typedef T_Tree_Node(*T_N_func)(T_Tree_Node *, unsigned int);

T_N_func get_numeric_func(char *repr) {
    switch (strlen(repr)) {
        case 1:
            switch (repr[0]) {
                case '+':
                    return &N_add;
                case '-':
                    return &N_sub;
                case '*':
                    return &N_times;
                case '/':
                    return &N_div;
                case '^':
                    return &N_pow;
                case '%':
                    return &N_percent;
            }
    }
}


#endif  // FUNCTIONS_H
