/* For nicely-formatted error messages */

#include <stdlib.h>
#include <stdio.h>

#ifndef ERRORS_H
#define ERRORS_H

/*
 * err
 * ---
 * Prints a message and exits with code `EXIT_FAILURE`.
 *
 * Args:
 *  char *msg: The message to output.
 */
void err(char *msg) {
    puts(msg);
    exit(EXIT_FAILURE);
}

/*
 * err_red
 * -------
 * Prints a message, in red, and exits with code `EXIT_FAILURE`.
 *
 * Args:
 *  char *msg: The message to output.
 */
void err_red(char *msg) {
    printf("\033[91m\033[1m%s\033[0m\n", msg);
    exit(EXIT_FAILURE);
}

/*
 * err_red_location
 * ----------------
 * Prints a message, in colour, and exits with code `EXIT_FAILURE`.
 *
 * Args:
 *  char *location: The location of the error. Ideal format is: '[filename; function name]'.
 *  char *msg: The message to output.
 */
void err_red_location(char *location, char *msg) {
    printf("\033[2m%s\033[0m \033[91m\033[1m%s\033[0m\n", location, msg);
    exit(EXIT_FAILURE);
}

#endif  // ERRORS_H
