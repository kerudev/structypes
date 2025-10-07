// str.h - Utilities for string manipulation.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STR_IMPLEMENTATION: implementations of string functions.

#ifndef STR_H_
#define STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/**
 * Obtains the character at `n`. Supports negative indexing.
 * Creates an internal copy of `str` so it isn't mutated.
 * 
 * Returns `\0` when:
 * - `n > strlen(str)`
 */
char str_char_at(char *str, int n);

/**
 * Splits `str` using the characters in `sep`.
 * Returns an array of pointers to strings with a length of `total`.
 * 
 * Returns `NULL` when:
 * - `strdup` fails to duplicate `str`.
 * - `realloc` for `tokens` (internal variable) fails.
 */
char **str_split(char *str, const char *sep, size_t *total);

/**
 * Splits `str` using the characters in `sep` and returns the first result.
 * Creates an internal copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `strdup` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `strdup` fails to duplicate `first` (returned value).
 */
char *str_split_first(char *str, const char *sep);

/**
 * Splits `str` using the characters in `sep` and returns the last result.
 * Creates an internal copy of `str` so it isn't mutated.
 * 
 * Returns `NULL` when:
 * - `strdup` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `strdup` fails to duplicate `first` (returned value).
 */
char *str_split_last(char *str, const char *sep);

#endif // STR_H_

#if !defined(__STR_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(STR_IMPLEMENTATION))
#define __STR_IMPLEMENTED

char str_char_at(char *str, int n) {
    int size = (int)strlen(str);

    if (n > size) {
        printf("Index %d out of bounds for string of size %zu\n", n, size);
        return '\0';
    }

    // TODO check for negative index overflow

    return (n < 0) ? str[size + n] : str[n];
}

char **str_split(char *str, const char *sep, size_t *total) {
    char *tmp = strdup(str);
    if (tmp == NULL) return NULL;

    char **tokens = NULL;
    size_t size = sizeof(char *);
    size_t cap = 0;

    char *token = strtok(tmp, sep);
    while (token) {
        char **new_tokens = realloc(tokens, (cap + 1) * size);

        if (!new_tokens) {
            free(tokens);
            free(tmp);
            return NULL;
        }

        tokens = new_tokens;
        tokens[cap++] = strdup(token);
        token = strtok(NULL, sep);
    }

    *total = cap;
    return tokens;
}

char *str_split_first(char *str, const char *sep) {
    char *tmp = strdup(str);
    if (tmp == NULL) return NULL;

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        return NULL;
    }

    char *first = strdup(token);
    free(tmp);

    return first;
}

char *str_split_last(char *str, const char *sep) {
    char *tmp = strdup(str);
    if (tmp == NULL) return NULL;

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        return NULL;
    }

    char *last;
    while (token) {
        last = strdup(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

    return last;
}

#endif // STR_IMPLEMENTATION
