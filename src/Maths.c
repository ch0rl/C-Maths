/* A program to evaluate maths expressions */

#include <stdio.h>
#include <stdbool.h>

#include "Tokenise.h"
#include "ToTree.h"
#include "Evaluate.h"
#include "Symbols.h"

#define INPUT_BUFF 50
#define INPUT_BUFF_INC 20

#define HELP_MSG "\n\
C-Maths\n\
-------\n\
Simply input a mathematical expression and receive a simplified version as a tree.\n\n\
Example:\n\
Expression: (4 + 9) * (a - 2)\n\
(4 + 9) * (a - 2) ≡ \n\
                    *\n\
                      13.000000\n\
                      -\n\
                        a\n\
                        2\n\
"

/*
 * get_input
 * ---------
 * Returns a string of characters from stdin, until `terminator`.
 *
 * Args:
 *  char terminator: The terminator to return at.
 *
 * Returns:
 *  char *: The string from stdin; this is allocated with malloc so will need
 *      to be 'free'd.
 */
char *get_input(char terminator) {
    char *buff = (char *)malloc(sizeof(char) * INPUT_BUFF);
    unsigned int len = 0;
    unsigned int buff_len = INPUT_BUFF;

    for (char c = getc(stdin); c != terminator; c = getc(stdin)) {
        if (len + 1 == buff_len) {
            buff_len += INPUT_BUFF_INC;
            buff = (char *)realloc(buff, buff_len);
        }

        buff[len++] = c;
    }

    buff[len] = 0;
    return buff;
}

/*
 * print_tree
 * ----------
 * Prints a tree, in-order.
 *
 * Args:
 *  T_Tree_Node head: The head node.
 *  unsigned int depth: The current depth (start at 0).
 *  unsigned int start_indent: The inital indentation.
 */
void print_tree(T_Tree_Node head, unsigned int depth, unsigned int start_indent) {
    for (unsigned int i = 0; i < start_indent + (2 * depth); i++) {
        printf(" ");
    }

    printf("%s\n", head.token.repr);

    if (head.c_num > 0) {
        for (unsigned int i = head.c_num - 1; i > 0; i--) {
            print_tree(head.children[i], depth + 1, start_indent);
        }
        print_tree(head.children[0], depth + 1, start_indent);
    }
}

int main(char *argv[], int argc) {
    unsigned int sym_table_buff, sym_n = 0;
    T_Symbol *sym_table = new_sym_table(&sym_table_buff);

    int assigned = 0;
    
    char cont = 'y';
    while (cont == 'y' || cont == 'Y') {
        printf("Expression: ");
        char *input = get_input('\n');

        // Check for help (input[0] *will* be defined)
        if (input[0] == '?') {
            puts(HELP_MSG);
        } else {
            unsigned int tok_num;
            T_Token *tokens = Tokenise(input, &tok_num);
            T_Tree_Node head = Form_tree(tokens, &tok_num);

            // Check assignments
            if (head.token.type == T_OP && head.token.repr[0] == '=') {
                int assigned = handle_assignment(head, sym_table, &sym_n, &sym_table_buff);
            } else {
                int assigned = S_NO_ASSIGNMENT;                
                head = Evaluate(head, sym_table, sym_n);
            }
            
            printf("\"%s\" ≡ \n", input);
            print_tree(head, 0, strlen(input) + 5);
            puts("");

            // Only free if not needed anymore
            if (assigned == S_NO_ASSIGNMENT) {
                free_tokens(tokens, tok_num);
                free_tree(head);
            }
        }   

        printf("Continue? \033[2m[y/N]\033[0m ");
        if ((cont = fgetc(stdin)) != '\n') {
            // Consume newline
            while (fgetc(stdin) != '\n') { }
        }
    }
}
