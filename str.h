// str.h - Utilities for string manipulation.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
// - STR_IMPLEMENTATION: implementations of string functions.
//
// Function macros:
// - str_concat
// - str_join

#ifndef STR_H_
#define STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>

// TODO replace strtok to get rid of this include
#include <string.h>

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
 * Returns `s1` and `s1` concatenated into one string.
 *
 * This function is not meant to be called directly, but by other functions in
 * the str_concat family.
 *
 * Returns `NULL` when:
 * - `s1` or `s2` are `NULL`.
 * - `realloc` fails.
 */
char *__str_concat(char *s1, char *s2);

/**
 * Returns all strings in `arr` concatenated into one string.
 * `size` is the total of elements in `arr` and is needed to iterate it.
 *
 * If you don't want to pass `size`, consider using `str_concat`.
 *
 * Returns `NULL` when:
 * - `arr` or `size` evaluate to false.
 */
char *str_concat_arr(char **arr, size_t size);

/**
 * Returns a string that joins all elements in `arr` using `joiner`.
 *
 * Returns `NULL` when:
 * - `joiner`, `arr` or `size` evaluate to false.
 * - `str_concat` fails to join `s1` and `s2`.
 */
char *str_join_arr(char *joiner, char **arr, size_t size);

/**
 * Removes characters on the left side of `s` using `isspace`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`
 */
char *str_ltrim(char *s);

/**
 * Removes characters on the right side of `s` using `isspace`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`
 */
char *str_rtrim(char *s);

/**
 * Calls `str_rtrim`, then `str_ltrim`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`
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
    int len = (int)str_len(str);

    if (abs(n) > len)
        THROW('\0', "str_char_at: Index %d out of bounds for string of size %zu", n, len);

    return (n < 0) ? str[len + n] : str[n];
}

char **str_split(char *str, const char *sep, size_t *total) {
    *total = 0;

    char *tmp = str_clone(str);
    if (tmp == NULL) THROW(NULL, "str_split: error cloning str");

    char **tokens = NULL;
    size_t size = sizeof(char *);

    char *token = strtok(tmp, sep);
    while (token) {
        char **new_tokens = realloc(tokens, (*total + 1) * size);
        if (!new_tokens) {
            free(tokens);
            free(tmp);
            THROW(NULL, "str_split: realloc error");
        }

        tokens = new_tokens;
        tokens[(*total)++] = str_clone(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

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

char *__str_concat(char *s1, char *s2) {
    if (s1 == NULL) THROW(NULL, "__str_concat: s1 can't be NULL");
    if (s2 == NULL) THROW(NULL, "__str_concat: s2 can't be NULL");

    size_t len = str_len(s1);

    char *buf = realloc(str_clone(s1), len + str_len(s2) + 1);
    if (!buf) THROW(NULL, "__str_concat: realloc error");

    char *ptr = buf + len;

    for (const char *cur = s2; *cur; cur++) *ptr++ = *cur;
    *ptr = '\0';

    return buf;
}

char *str_concat_arr(char **arr, size_t size) {
    if (!arr) THROW(NULL, "str_concat_arr: arr evaluates to false");
    if (!size) THROW(NULL, "str_concat_arr: size can't be 0");

    char *buf = str_clone(arr[0]);
    if (!buf) THROW(NULL, "str_concat_arr: str_clone failed");

    for (size_t i = 1; i < size; i++) {
        char *tmp = __str_concat(buf, arr[i]);
        if (!tmp) {
            free(buf);
            THROW(NULL, "str_concat_arr: __str_concat error on loop");
        }

        buf = tmp;
    }

    return buf;
}

#define str_concat(...) \
    str_concat_arr((char *[]){ __VA_ARGS__ }, sizeof((char *[]){ __VA_ARGS__ }) / sizeof(char *));

char *str_join_arr(char *joiner, char **arr, size_t size) {
    if (!joiner) THROW(NULL, "str_join_arr: joiner evaluates to false");
    if (!arr) THROW(NULL, "str_join_arr: arr evaluates to false");
    if (!size) THROW(NULL, "str_join_arr: size can't be 0");

    char *buf = str_clone(arr[0]);
    if (!buf) THROW(NULL, "str_join_arr: str_clone failed");

    char *arg;
    for (size_t i = 1; i < size; i++) {
        char *tmp = str_concat(buf, joiner, arr[i]);
        if (!tmp) {
            free(buf);
            THROW(NULL, "str_join_arr: str_concat failed to join with %s", arg);
        }

        buf = tmp;
    }

    return buf;
}

#define str_join(joiner, ...) \
    str_join_arr(joiner, (char *[]){ __VA_ARGS__ }, sizeof((char *[]){ __VA_ARGS__ }) / sizeof(char *));

char *str_ltrim(char *s) {
    if (s == NULL) THROW(NULL, "str_ltrim: s can't be NULL");
    while(isspace(*s)) s++;
    return s;
}

char *str_rtrim(char *s) {
    if (s == NULL) THROW(NULL, "str_rtrim: s can't be NULL");

    int len = str_len(s);
    if (len == 0) return s;

    char* back;
    back = s + len - 1;

    while (back >= s && isspace(*back)) back--;
    *(back+1) = '\0';

    return s;
}

char *str_trim(char *s) {
    if (s == NULL) THROW(NULL, "str_trim: s can't be NULL");
    return str_ltrim(str_rtrim(s));
}

#endif // STR_IMPLEMENTATION
