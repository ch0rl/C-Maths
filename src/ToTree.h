/* For compiling tokens to trees */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Tokenise.h"
#include "Structures.h"


#ifndef TO_TREE_H
#define TO_TREE_H

typedef struct OP_FUNC_ARG {
    char *repr;
    unsigned int len;
    unsigned int arg_num;
} T_OP_FUNC_ARG;

#define N_ARG_LOOKUP_LEN 6
// {<op/func>, <op/func length>, <number of arguments>}
T_OP_FUNC_ARG N_ARG_LOOKUP[N_ARG_LOOKUP_LEN] = {
    {"+", 1, 2},
    {"-", 1, 2},
    {"*", 1, 2},
    {"/", 1, 2},
    {"^", 1, 2},
    {"=", 1, 2}
};

/*
 * precedence
 * ----------
 * Returns the precedence of an operator.
 *
 * Args:
 *  char c: The operator to check.
 *
 * Return:
 *  int: The relative (signed) precedence of the operator.
 */
int precedence(char c) {
    switch (c) {
        case '=':
            return 10;
        case '^':
            return 0;
        case '*':
        case '/':
            return 1;
        case '+':
        case '-':
            return 2;
        default:
            return 3;
    }
}

/*
 * n_args
 * ------
 * Returns how many arguments an operator/function takes.
 *
 * Args:
 *  char *repr: The string representation of the operator/function.
 *
 * Return:
 *  int: The number of arguments (exits on error).
 */
int n_args(char *repr) {
    int repr_len = strlen(repr);
    for (unsigned int i = 0; i < N_ARG_LOOKUP_LEN; i++) {
        if (repr_len == N_ARG_LOOKUP[i].len) {
            if (strncmp(repr, N_ARG_LOOKUP[i].repr, repr_len) == 0) {
                return N_ARG_LOOKUP[i].arg_num;
            }
        }
    }

    err_red_location("[ToTree.h; n_args]", "Failed to lookup n_args.");
}

// Prototype
void __shunting_yard(T_Token *, unsigned int *);

/*
 * Form_tree
 * ---------
 * Forms a tree from an array of tokens.
 *
 * Args:
 *  T_Token *tokens: The array of tokens to parse.
 *  unsigned int *tok_num: The number of tokens.
 *
 * Return:
 *  T_Tree_Node: The head of the new tree.
 */
T_Tree_Node Form_tree(T_Token *tokens, unsigned int *tok_num) {
    // Get postfix
    __shunting_yard(tokens, tok_num);

    // Now form tree
    T_Node_Stack *node_stack = new_node_stack(20);
    for (unsigned int i = 0; i < *tok_num; i++) {
        T_Token tok = tokens[i];
        switch (tok.type) {
            case T_NUM:
            case T_SYM:
                stack_push_node(node_stack, new_tree_node(tok));
                break;

            case T_OP:
            case T_FUNC:
                int arg_n = n_args(tok.repr);
                T_Tree_Node *children = (T_Tree_Node *)malloc(sizeof(T_Tree_Node) * arg_n);
                for (int i = 0; i < arg_n; i++) {
                    children[i] = stack_pop_node(node_stack);
                }

                T_Tree_Node new_node = { tok, arg_n, arg_n, children };
                stack_push_node(node_stack, new_node);
        }
    }

    return stack_pop_node(node_stack);
}

/* https://en.wikipedia.org/wiki/Shunting_yard_algorithm
    
*/
/*
 * __shunting_yard
 * ---------------
 * Performs the 'shunting yard' algorithm on an array of tokens -- in-place.
 * See https://en.wikipedia.org/wiki/Shunting_yard_algorithm.
 *
 * Args:
 *  T_Token *tokens: The array of tokens.
 *  unsigned int *tok_num: The number of tokens.
 */
void __shunting_yard(T_Token *tokens, unsigned int *tok_num) {
    T_Stack *op_stack = new_stack(20);  // Resizes automatically
    T_Queue *out_queue = new_queue(*tok_num + 1);

    for (unsigned int i = 0; i < *tok_num; i++) {
        T_Token tok = tokens[i];
        switch (tok.type) {
            case T_NUM:
            case T_SYM:
                enqueue(out_queue, tok);
                break;

            case T_OPEN:
            case T_FUNC:
                stack_push(op_stack, tok);
                break;

            case T_OP:
                while (op_stack->len != 0 && stack_peek(op_stack).type != T_OPEN
                    && precedence(tok.repr[0]) > precedence(stack_peek(op_stack).repr[0])
                    ) {
                    enqueue(out_queue, stack_pop(op_stack));
                }
                stack_push(op_stack, tok);
                break;

            case T_CLOSE:
                while (stack_peek(op_stack).type != T_OPEN) {
                    enqueue(out_queue, stack_pop(op_stack));
                }
                stack_pop(op_stack);

                if (op_stack->len > 0 && stack_peek(op_stack).type == T_FUNC) {
                    enqueue(out_queue, stack_pop(op_stack));
                }
                break;
        }
    }

    while (op_stack->len != 0) {
        enqueue(out_queue, stack_pop(op_stack));
    }

    // Copy queue to tokens
    for (unsigned int i = 0; i < out_queue->top; i++) {
        tokens[i] = out_queue->_queue[i];
    }
    *tok_num = out_queue->top;
}

#endif  // TO_TREE_H
