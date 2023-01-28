/* For evaluating trees */

#include "Structures.h"
#include "Functions.h"

#ifndef EVALUATE_H
#define EVALUATE_H

#define ASSOCIATIVE_N 2
// The operators that are associative
char *ASSOCIATIVE[2] = {
    "+", "-"
};

/*
 * __replace_symbol
 * ----------------
 * Replaces `node` with its value in `sym_table`, if it exists.
 * `node.token.type` must be T_SYM
 *
 * Args:
 *  T_Tree_Node node: The symbol node to lookup & replace.
 *  T_Symbol *sym_table: The symbol table.
 *  unsigned int sym_num: The number of symbols in the table.
 *
 * Return:
 *  T_Tree_Node: The new, if changed, node; the original, otherwise.
 */
T_Tree_Node __replace_symbol(T_Tree_Node node, T_Symbol *sym_table, unsigned int sym_num) {
    for (unsigned int i = 0; i < sym_num; i++) {
        if (strcmp(sym_table[i].repr, node.token.repr) == 0) {
            if (sym_table[i].has_val) {
                node.token.data = sym_table[i].val;
                node.token.repr = double_to_string(sym_table[i].val);
                node.token.type = T_NUM;

                return node;
            } else if (sym_table[i].has_expr) {
                return sym_table[i].expr;
            }
        }
    }

    return node;
}

/*
 * __parse_numbers
 * ---------------
 * Parse and evaluate all operators/functions who's children are numbers.
 *
 * Args:
 *  T_Tree_Node head: The head of the tree.
 *
 * Return:
 *  T_Tree_Node: The new tree head.
 */
T_Tree_Node __parse_numbers(T_Tree_Node head) {
    if (head.token.type != T_OP && head.token.type != T_FUNC) {
        return head;
    }
    
    for (unsigned int i = 0; i < head.c_num; i++) {
        if (head.children[i].token.type != T_NUM) {
            return head;
        }
    }

    // All children are numbers
    T_N_func func = get_numeric_func(head.token.repr);
    return func(head.children, head.c_num);
}

/*
 * Evaluate
 * --------
 * Evaluate a tree.
 *
 * Args:
 *  T_Tree_Node head: The head of the tree.
 *  T_Symbol *sym_table: The symbol table.
 *  unsigned int sym_num: The number of symbols in the table.
 */
T_Tree_Node Evaluate(T_Tree_Node head, T_Symbol *sym_table, unsigned int sym_num) {
    for (unsigned int i = 0; i < head.c_num; i++) {
        head.children[i] = Evaluate(head.children[i], sym_table, sym_num);
    }
    
    if (head.token.type == T_SYM) {
        head = __replace_symbol(head, sym_table, sym_num);
    }
    
    head = __parse_numbers(head);

    return head;
}

#endif  // EVALUATE_H
