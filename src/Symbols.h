/* For handling symbols */

#include "Structures.h"
#include "Errors.h"

#define SYM_TABLE_BUFF 10
#define SYM_TABLE_BUFF_INC 5

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
 */
void add_sym(T_Symbol *sym_table, T_Symbol new_sym, unsigned int *sym_n, unsigned int *buff_len) {
    if (sym_n == buff_len) {
        *buff_len += SYM_TABLE_BUFF_INC;
        sym_table = (T_Symbol *)realloc(sym_table, *buff_len);

        if (sym_table == NULL) {
            err_red_location("[Symbols.h; add_sym]", "Unable to reallocate sym_table.");
        }
    }

    sym_table[*sym_n++] = new_sym;
}
