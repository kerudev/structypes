// str.h - Utilities for string manipulation.
//
// All functions return a value for error handling:
// - int: 0 on failure, 1 on success.
// - void *: NULL on failure, void * on success.
//
// Useful macros:
// - STR_IMPLEMENTATION: implementations of the Vec structure.
// - STR_CAPACITY_STEP: used to initialize and realloc the Vec's capacity.

#ifndef STR_H_
#define STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/**
 * Obtains the character at `n`. Supports negative indexing.
 * Creates an internal copy of `str` so it isn't mutated.
 */
char str_char_at(char *str, int n);

/**
 * Splits `str` using the characters in `sep`.
 * Returns an array of pointers to strings with a length of `total`.
 */
char **str_split(char *str, const char *sep, size_t *total);

/**
 * Splits `str` using the characters in `sep` and returns the first result.
 * Creates an internal copy of `str` so it isn't mutated.
 */
char *str_split_first(char *str, const char *sep);

/**
 * Splits `str` using the characters in `sep` and returns the last result.
 * Creates an internal copy of `str` so it isn't mutated.
 */
char *str_split_last(char *str, const char *sep);

#endif // STR_H_

#if defined(STR_IMPLEMENTATION) && !defined(__STR_ALREADY_IMPLEMENTED)
#define __STR_ALREADY_IMPLEMENTED

#ifndef STR_CAPACITY_STEP
#define STR_CAPACITY_STEP 128
#endif // STR_CAPACITY_STEP

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

    char *first = strdup(strtok(tmp, sep));

    free(tmp);

    return first;
}

char *str_split_last(char *str, const char *sep) {
    char *tmp = strdup(str);
    if (tmp == NULL) return NULL;

    char *token = strtok(tmp, sep);
    char *last;
    while (token) {
        last = strdup(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

    return last;
}

#endif // STR_IMPLEMENTATION
