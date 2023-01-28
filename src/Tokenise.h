/* For tokenising expressions */


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Errors.h"

#ifndef TOKENISER_H
#define TOKENISER_H

#define TOK_BUFF_N 30

enum TOKENS { T_NULL, T_FUNC, T_OP, T_SYM, T_OPEN, T_CLOSE, T_NUM };

typedef struct Token {
    unsigned int type;  // See above

    char *repr;  // If type == func, op, sym, brace
    long double data;  // If type == num
} T_Token;

char OPERATORS[] = { '+', '-', '*', '/', '^', '=' };
#define OP_NUM 6


/* Returns if `x` is in `OPERATORS` */
bool is_op(char x) {
    for (unsigned int i = 0; i < OP_NUM; i++) {
        if (x == OPERATORS[i]) {
            return true;
        }
    }

    return false;
}

/*
 * is_digit
 * --------
 * Returns whether `x` is a digit.
 *
 * Args:
 *  char x: The character to check.
 *
 * Return:
 *  bool: Whether `x` is a digit.
 */
bool is_digit(char x) {
    return '0' <= x && x <= '9';
}

/*
 * is_num
 * --------
 * Returns whether `x` is a digit or a period ('.').
 *
 * Args:
 *  char x: The character to check.
 *
 * Return:
 *  bool: Whether `x` is a digit or period.
 */
bool is_num(char x) {
    return x == '.' || is_digit(x);
}

/*
 * is_open
 * --------
 * Returns whether `x` is an open brace.
 *
 * Args:
 *  char x: The character to check.
 *
 * Return:
 *  bool: Whether `x` is an open brace.
 */
bool is_open(char x) {
    return x == '(' || x == '[' || x == '{';
}

/*
 * is_close
 * --------
 * Returns whether `x` is a close brace.
 *
 * Args:
 *  char x: The character to check.
 *
 * Return:
 *  bool: Whether `x` is a close brace.
 */
bool is_close(char x) {
    return x == ')' || x == ']' || x == '}';
}

/*
 * is_letter
 * --------
 * Returns whether `x` is an latin letter.
 *
 * Args:
 *  char x: The character to check.
 *
 * Return:
 *  bool: Whether `x` is a letter.
 */
bool is_letter(char x) {
    return ('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z');
}

/*
 * add_token
 * ---------
 * Adds a token to a list of tokens, resizing if necessary.
 *
 * Args:
 *  T_Token *tokens: The array to append to.
 *  T_Token new_token: The token to append.
 *  unsigned int *tok_num: The number of tokens (updated as necessary).
 *  unsigned int *tok_buff: The length of the token buffer (updated as necessary).
 */
void add_token(T_Token *tokens, T_Token new_token, unsigned int *tok_num, unsigned int *tok_buff) {
    if (*tok_num == *tok_buff) {
        *tok_buff += TOK_BUFF_N;
        tokens = (T_Token *)realloc(tokens, sizeof(T_Token) * (*tok_buff));

        if (tokens == NULL) {
            err_red_location("[Tokeniser.h; add_token]", "Failed to allocate tokens.");
        }
    }

    tokens[(*tok_num)++] = new_token;
}

/*
 * free_tokens
 * -----------
 * Frees an array of tokens.
 *
 * Args:
 *  T_Token *tokens: The array of tokens to free.
 *  unsigned int t_num: The number of tokens.
 */
void free_tokens(T_Token *tokens, unsigned int t_num) {
    for (unsigned int i = 0; i < t_num; i++) {
        free(tokens[i].repr);
    }

    free(tokens);
}

/*
 * Tokenise
 * --------
 * Forms an array of tokens from an expression.
 *
 * Args:
 *  char *expr: The expression to form tokens from.
 *  unsigned int *tok_num: A pointer to the final number of tokens.
 *
 * Return:
 *  T_Token *: The array of tokens.
 */
T_Token *Tokenise(char *expr, unsigned int *tok_num) {
    T_Token *tokens = (T_Token *)malloc(sizeof(T_Token) * TOK_BUFF_N);
    if (tokens == NULL) {
        err_red_location("[Tokeniser.h; Tokenise]", "Failed to allocate tokens.");
    }

    *tok_num = 0;
    unsigned int tok_buff_len = TOK_BUFF_N;

    unsigned int expr_len = strlen(expr);
    char c;
    for (unsigned int i = 0; i < expr_len; i++) {
        c = expr[i];

        if (is_digit(c)) {
            // Numbers
            unsigned int x = i;
            bool had_point = false;

            // Find length of number
            while (x < expr_len && is_num(expr[x])) {
                if (expr[x] == '.') {
                    if (had_point) {
                        // Can't have multiple '.' in one number
                        err_red_location("[Tokeniser.h; Tokenise]", "Cannot have >1 '.'s in one number.");
                    }
                    had_point = true;
                }
                x++;
            }

            // Get number and save
            T_Token tok = { 0, NULL, 0.0l };
            tok.type = T_NUM;
            tok.repr = (char *)malloc(sizeof(char) * (x - i + 1));
            if (tok.repr == NULL) {
                err_red_location("[Tokeniser.h; Tokenise]", "Failed to allocate tok.repr.");
            }
            
            unsigned int j = 0;
            for (; j < (x - i); j++) {
                tok.repr[j] = expr[i + j];
            }
            tok.repr[j + 1] = 0;
            tok.data = strtold(tok.repr, NULL);

            add_token(tokens, tok, tok_num, &tok_buff_len);

            // Set i to new value
            i = x - 1;
        } else if (is_op(c) || is_open(c) || is_close(c)) {
            // Operators & Braces
            T_Token tok = { 0, NULL, 0.0 };

            if (is_op(c)) {
                tok.type = T_OP;
            } else if (is_open(c)) {
                tok.type = T_OPEN;
            } else {
                tok.type = T_CLOSE;
            }

            tok.repr = (char *)malloc(sizeof(char) * 2);
            if (tok.repr == NULL) {
                err_red_location("[Tokeniser.h; Tokenise]", "Failed to allocate tok.repr.");
            }
            
            tok.repr[0] = c;
            tok.repr[1] = 0;

            add_token(tokens, tok, tok_num, &tok_buff_len);
        } else if (is_letter(c)) {
            // Symbols / Funcs
            unsigned int n_len = 0;
            unsigned int x = i;

            // Find length of number
            while (x < expr_len && is_letter(expr[x])) { x++; }

            // Get string and save
            T_Token tok = { 0, NULL, 0.0 };
            tok.type = T_SYM;
            tok.repr = (char *)malloc(sizeof(char) * (x - i + 1));
            if (tok.repr == NULL) {
                err_red_location("[Tokeniser.h; Tokenise]", "Failed to allocate tok.repr.");
            }

            unsigned int j = 0;
            for (; j < (x - i); j++) {
                tok.repr[j] = expr[i + j];
            }
            tok.repr[j + 1] = 0;
            
            add_token(tokens, tok, tok_num, &tok_buff_len);

            // Set i to new value
            i = x - 1;
        }
    }

    return tokens;
}


#endif  // TOKENISER_H
