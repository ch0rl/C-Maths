/* For handling symbols */

#include "Structures.h"
#include "Errors.h"

#define SYM_TABLE_BUFF 10
#define SYM_TABLE_BUFF_INC 5

enum { S_ASSIGNED_NEW, S_UPDATED_OLD, S_NO_ASSIGNMENT };

/*
 * new_sym_table
 * -------------
 * Allocates a new symbol table.
 *
 * Args:
 *  unsigned int *buff_len: A pointer to the length of the buffer; this is updated.
 *
 * Return:
 *  T_Symbol *: The array of symbols.
 */
T_Symbol *new_sym_table(unsigned int *buff_len) {
    T_Symbol *table = (T_Symbol *)malloc(sizeof(T_Symbol) * SYM_TABLE_BUFF);

    if (table == NULL) {
        err_red_location("[Symbols.h; new_sym_table]", "Failed to allocate a new table.");
    }

    *buff_len = SYM_TABLE_BUFF;
    return table;
}

/*
 * add_sym
 * -------
 * Adds a symbol to an existing symbol table.
 *
 * Args:
 *  T_Symbol *sym_table: The array of existing symbols (updated herein)
 *  T_Symbol new_sym: The new symbol to add
 *  unsigned int *sym_n: A pointer to the number of symbols (updated herein)
 *  unsigned int *bufF_len: A pointer to the size of the buffer (sym_table) (updated herein)
 */
void add_sym(T_Symbol *sym_table, T_Symbol new_sym, unsigned int *sym_n, unsigned int *buff_len) {
    if (sym_n == buff_len) {
        *buff_len += SYM_TABLE_BUFF_INC;
        sym_table = (T_Symbol *)realloc(sym_table, *buff_len);

        if (sym_table == NULL) {
            err_red_location("[Symbols.h; add_sym]", "Unable to reallocate sym_table.");
        }
    }

    sym_table[(*sym_n)++] = new_sym;
}

/*
 * handle_assignment
 * -----------------
 * Handles the assignment of values/expressions to symbols.
 *
 * Args:
 *  T_Tree_Node head: The head of the tree, as returned by ToTree.h
 *  T_Symbol *sym_table: The array of existing symbols (updated herein)
 *  unsigned int *sym_n: A pointer to the number of symbols (updated herein)
 *  unsigned int *buff_len: A pointer to the size of the buffer (sym_table) (updated herein)
 *
 * Return:
 *  bool: Whether an assignment took place:
 *          0 = Made a new symbol
 *          1 = Updated a symbol
 *          2 = Nothing happened
 */
int handle_assignment(T_Tree_Node head, T_Symbol *sym_table, unsigned int *sym_n, unsigned int *buff_len) {
    // Should only be 2 children (children order is reversed)
    if (head.children[1].token.type == T_SYM) {
        T_Tree_Node new_sym = head.children[1];
        bool numeric = head.children[0].token.type == T_NUM;

        // Search if it exists
        for (unsigned int i = 0; i < *sym_n; i++) {
            if (strcmp(sym_table[i].repr, new_sym.token.repr) == 0) {
                T_Symbol sym = sym_table[i];
                // Sym already exists -- update
                if (numeric) {
                    if (sym.has_expr) {
                        sym.has_expr = false;
                        free_tree(sym.expr);
                    }

                    sym.has_val = true;
                    sym.val = head.children[0].token.data;
                } else {
                    sym.has_val = false;
                    sym.has_expr = true;

                    // RHS of head /should/ be op/func 
                    sym.expr = head.children[0];
                }

                return S_UPDATED_OLD;
            }
        }

        // Make new symbol
        T_Symbol sym;
        sym.repr = new_sym.token.repr;
            
        sym.has_expr = !numeric;
        sym.has_val = numeric;

        if (numeric) {
            sym.val = head.children[0].token.data;
        } else {
            // RHS of head /should/ be op/func 
            sym.expr = head.children[0];
        }

        add_sym(sym_table, sym, sym_n, buff_len);
        return S_ASSIGNED_NEW;
    }

    return S_NO_ASSIGNMENT;
}
