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
#include <stdarg.h>
#include <stddef.h>

/**
 * `strlen` replacement. Returns the size of `s`.
 * 
 * Returns `0` when:
 * - `s` evaluates to false.
 */
size_t str_len(char *s);

/**
 * `strdup` replacement. Returns a clone of `s`.
 * 
 * Returns `NULL` when:
 * - `s` evaluates to false.
 * - `malloc` fails.
 */
char *str_clone(char *s);

/**
 * Obtains the character at `n`. Supports negative indexing.
 * Creates an internal copy of `str` so it isn't mutated.
 *
 * Returns `\0` when:
 * - `n > str_len(str)`
 */
char str_char_at(char *str, int n);

/**
 * Splits `str` using the characters in `sep`.
 * Returns an array of pointers to strings with a length of `total`.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `realloc` for `tokens` (internal variable) fails.
 */
char **str_split(char *str, const char *sep, size_t *total);

/**
 * Splits `str` using the characters in `sep` and returns the first result.
 * Creates an internal copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_first(char *str, const char *sep);

/**
 * Splits `str` using the characters in `sep` and returns the last result.
 * Creates an internal copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_last(char *str, const char *sep);

/**
 * Returns all strings in `arr` concatenated into one string.
 * `size` is the total of elements in `arr` and is needed to iterate it.
 *
 * If you don't want to pass `size`, consider using `str_concat_va`.
 *
 * Returns `NULL` when:
 * - `arr` or `size` evaluate to false.
 * - `malloc` or `realloc` fail.
 */
char *str_concat(char **arr, size_t size);

/**
 * Returns all arguments concatenated into one string.
 *
 * Variadic arguments must end with `NULL` to stop iteration.
 *
 * Returns `NULL` when:
 * - `first` or `second` are `NULL`.
 * - `malloc` or `realloc` fail.
 */
char *str_concat_va(char *first, char *second, ...);

#endif // STR_H_

#if !defined(__STR_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(STR_IMPLEMENTATION))
#define __STR_IMPLEMENTED

size_t str_len(char *s) {
    if (!s) return 0;

    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char *str_clone(char *s) {
    if (!s) return NULL;

    size_t len = str_len(s) + 1;
    char *copy = malloc(len);
    if (!copy) return NULL;

    for (size_t i = 0; i < len; i++) copy[i] = s[i];

    return copy;
}

char str_char_at(char *str, int n) {
    int size = (int)str_len(str);

    if (n > size) {
        printf("Index %d out of bounds for string of size %zu\n", n, size);
        return '\0';
    }

    // TODO check for negative index overflow

    return (n < 0) ? str[size + n] : str[n];
}

char **str_split(char *str, const char *sep, size_t *total) {
    char *tmp = str_clone(str);
    if (tmp == NULL) return NULL;

    char **tokens = NULL;
    size_t size = sizeof(char *);
    // TODO cap -> total
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
        tokens[cap++] = str_clone(token);
        token = strtok(NULL, sep);
    }

    *total = cap;
    return tokens;
}

char *str_split_first(char *str, const char *sep) {
    char *tmp = str_clone(str);
    if (tmp == NULL) return NULL;

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        return NULL;
    }

    char *first = str_clone(token);
    free(tmp);

    return first;
}

char *str_split_last(char *str, const char *sep) {
    char *tmp = str_clone(str);
    if (tmp == NULL) return NULL;

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        return NULL;
    }

    char *last;
    while (token) {
        last = str_clone(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

    return last;
}

char *str_concat(char **arr, size_t size) {
    if (!arr || !size) return NULL;

    char *buf = malloc(str_len(arr[0]) + 1);
    if (!buf) return NULL;

    strcpy(buf, arr[0]);

    size_t total = str_len(buf);
    for (size_t i = 1; i < size; i++) {
        size_t elementSize = str_len(arr[i]);

        char *tmp = realloc(buf, total + elementSize + 1);
        if (!tmp) {
            free(buf);
            return NULL;
        }

        buf = tmp;
        strcat(buf, arr[i]);
        total += elementSize;
    }

    return buf;
}

char *str_concat_va(char *first, char *second, ...) {
    if (first == NULL || second == NULL) return NULL;

    char *buf = malloc(str_len(first) + str_len(second) + 1);
    if (!buf) return NULL;

    strcpy(buf, first);
    strcat(buf, second);

    va_list args;
    va_start(args, second);

    char *arg;
    while ((arg = va_arg(args, char *))) {
        char *tmp = realloc(buf, str_len(buf) + str_len(arg) + 1);
        if (!tmp) {
            free(buf);
            va_end(args);
            return NULL;
        }

        buf = tmp;
        strcat(buf, arg);
    }

    va_end(args);

    return buf;
}

#endif // STR_IMPLEMENTATION
