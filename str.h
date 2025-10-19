// str.h - Utilities for string manipulation.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
// - STR_IMPLEMENTATION: implementations of string functions.

#ifndef STR_H_
#define STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>

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
 * Creates a copy of `str` so it isn't mutated.
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
 * Creates a copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_first(char *str, const char *sep);

/**
 * Splits `str` using the characters in `sep` and returns the last result.
 * Creates a copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `strtok` return a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_last(char *str, const char *sep);

/**
 * Returns all strings in `arr` concatenated into one string.
 *
 * If you prefer to use a fixed-size array, consider using `str_concat_arr`.
 * If you don't want to pass `size`, consider using `str_concat_va`.
 *
 * Returns `NULL` when:
 * - `s1` or `s2` are `NULL`.
 * - `realloc` fails.
 */
char *str_concat(char *s1, char *s2);

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
char *str_concat_arr(char **arr, size_t size);

/**
 * Returns all arguments concatenated into one string.
 *
 * Variadic arguments must end with `NULL` to stop iteration.
 *
 * If you prefer to use a fixed-size array, consider using `str_concat_arr`.
 *
 * Returns `NULL` when:
 * - An error occurred when calling `str_concat`.
 * - `malloc` or `realloc` fail.
 */
char *str_concat_va(char *s1, char *s2, ...);

/**
 * Returns a string that joins all arguments using `joiner`.
 *
 * Variadic arguments must end with `NULL` to stop iteration.
 * 
 * Returns `NULL` when:
 * - `str_concat_va` fails to join `s1` and `s2`.
 * - `str_concat_va` fails to join a variadic argument.
 */
char *str_join(char *joiner, char *s1, char *s2, ...);

/**
 * Removes characters on the left side of `s` using `isspace`.
 * Does not create a copy of `s`.
 */
char *str_ltrim(char *s);

/**
 * Removes characters on the right side of `s` using `isspace`.
 * Does not create a copy of `s`.
 */
char *str_rtrim(char *s);

/**
 * Calls `str_rtrim`, then `str_ltrim`.
 * Does not create a copy of `s`.
 */
char *str_trim(char *s);

#endif // STR_H_

#if !defined(__STR_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(STR_IMPLEMENTATION))
#define __STR_IMPLEMENTED

#ifdef STRUCTYPES_DEBUG
static void _str_err(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define THROW(ret, msg, ...) ({ _str_err(msg, ##__VA_ARGS__); return ret; })
#else
#define THROW(ret, msg, ...) ({ return ret; })
#endif

size_t str_len(char *s) {
    if (!s) THROW(0, "str_len: s evaluates to false");

    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char *str_clone(char *s) {
    if (!s) THROW(NULL, "str_clone: s evaluates to false");

    size_t len = str_len(s) + 1;
    char *copy = malloc(len);
    if (!copy) THROW(NULL, "str_clone: malloc error");

    for (size_t i = 0; i < len; i++) copy[i] = s[i];

    return copy;
}

char str_char_at(char *str, int n) {
    int size = (int)str_len(str);

    if (n > size)
        THROW('\0', "str_char_at: Index %d out of bounds for string of size %zu\n", n, size);

    // TODO check for negative index overflow

    return (n < 0) ? str[size + n] : str[n];
}

char **str_split(char *str, const char *sep, size_t *total) {
    char *tmp = str_clone(str);
    if (tmp == NULL) THROW(NULL, "str_split: error cloning str");

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
            THROW(NULL, "str_split: realloc error");
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
    if (tmp == NULL) THROW(NULL, "str_split_first: error cloning str");

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        THROW(NULL, "str_split_first: strtok error");
    }

    char *first = str_clone(token);
    free(tmp);

    return first;
}

char *str_split_last(char *str, const char *sep) {
    char *tmp = str_clone(str);
    if (tmp == NULL) THROW(NULL, "str_split_last: error cloning str");

    char *token = strtok(tmp, sep);
    if (token == NULL) {
        free(tmp);
        THROW(NULL, "str_split_last: strtok error");
    }

    char *last;
    while (token) {
        last = str_clone(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

    return last;
}

char *str_concat(char *s1, char *s2) {
    if (s1 == NULL) THROW(NULL, "str_concat: s1 can't be NULL");
    if (s2 == NULL) THROW(NULL, "str_concat: s2 can't be NULL");

    size_t len = str_len(s1);

    char *buf = realloc(str_clone(s1), len + str_len(s2) + 1);
    if (!buf) THROW(NULL, "str_append: realloc error");

    char *ptr = buf + len;

    for (const char *cur = s2; *cur; cur++) *ptr++ = *cur;
    *ptr = '\0';

    return buf;
}

char *str_concat_arr(char **arr, size_t size) {
    if (!arr) THROW(NULL, "str_concat_arr: arr evaluates to false");
    if (!size) THROW(NULL, "str_concat_arr: size can't be 0");

    char *buf = str_clone(arr[0]);
    if (!buf) THROW(NULL, "str_concat_arr: malloc error");

    for (size_t i = 1; i < size; i++) {
        char *tmp = str_concat(buf, arr[i]);
        if (!tmp) {
            free(buf);
            THROW(NULL, "str_concat_arr: realloc error");
        }

        buf = tmp;
    }

    return buf;
}

char *str_concat_va(char *s1, char *s2, ...) {
    char *buf = str_concat(s1, s2);
    if (!buf) THROW(NULL, "str_concat_va: str_concat error");

    va_list args;
    va_start(args, s2);

    char *arg;
    while ((arg = va_arg(args, char *))) {
        char *tmp = str_concat(buf, arg);
        if (!tmp) {
            free(buf);
            va_end(args);
            THROW(NULL, "str_concat_va: realloc error");
        }

        buf = tmp;
    }

    va_end(args);

    return buf;
}

char *str_join(char *joiner, char *s1, char *s2, ...) {
    va_list args;
    va_start(args, joiner);

    char *buf = str_concat_va(str_clone(s1), joiner, s2, NULL);
    if (!buf) THROW(NULL, "str_join: str_concat_va failed to join s1 and s2");

    char *arg;
    while ((arg = va_arg(args, char *))) {
        char *tmp = str_concat_va(buf, joiner, arg, NULL);
        if (!tmp) {
            free(buf);
            va_end(args);
            THROW(NULL, "str_join: str_concat_va failed to join with %s", arg);
        }

        buf = tmp;
    }

    va_end(args);

    return buf;
}

char *str_ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *str_rtrim(char *s) {
    int len = str_len(s);
    if (len == 0) return s;

    char* back;
    back = s + len - 1;

    while (back >= s && isspace(*back)) back--;
    *(back+1) = '\0';

    return s;
}

char *str_trim(char *s) {
    return str_ltrim(str_rtrim(s));
}

#endif // STR_IMPLEMENTATION
