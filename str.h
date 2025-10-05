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

/* Obtains the character at `n`. Supports negative indexing. */
char str_char_at(char *str, int n);

char *str_split(char *str, const char *sep);

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

char *str_split(char *str, const char *sep) {
    char *tmp = strdup(str);

    char *tokens = strtok(tmp, sep);
    while (tokens) tokens = strtok(NULL, sep);

    return tokens;
}

#endif // STR_IMPLEMENTATION
