// str.h - Utilities for string manipulation.
//
// Library macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
//
// Function macros:
// - str_concat
// - str_join

#ifndef STR_H_
#define STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// TODO don't create copies of the string where it's not needed

/**
 * `strlen` replacement.
 *
 * Returns the size of `s`.
 *
 * Returns `0` when:
 * - `s` evaluates to false.
 */
size_t str_len(char *s);

/**
 * `strdup` replacement.
 *
 * Returns a clone of `s`.
 *
 * Returns `NULL` when:
 * - `s` evaluates to false.
 * - `malloc` fails to allocate a copy of `s`.
 */
char *str_clone(char *s);

/**
 * Frees `s` and assigns it to `NULL` to avoid double-freeing.
 */
bool str_free(char **s);

/**
 * Obtains the character at `n`. Supports negative indexing.
 * Creates a copy of `str` so it isn't mutated.
 *
 * Returns `\0` when:
 * - `n > str_len(str)`.
 */
char str_char_at(char *str, int n);

/**
 * `isspace` replacement.
 *
 * Returns `true` when `c` equals to any of the following characters:
 * - ` ` (space)
 * - `\n`
 * - `\t`
 * - `\r`
 * - `\f`
 * - `\v`
 */
bool str_char_is_space(unsigned char c);

/**
 * `strtok` replacement.
 *
 * Returns the first token encountered in `str` separated by `sep`.
 * Doesn't create a copy of `str`. Its pointer gets advanced on each call.
 *
 * This function is not meant to be called directly, but by other functions in
 * the `str_split` family.
 *
 * Returns `NULL` when:
 * - `str` is not a pointer.
 * - `str` value evaluates to false.
 * - `sep` evaluates to false.
 */
char *__str_tok(char **str, const char *sep);

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
 * - `__str_tok` returns a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_first(char *str, const char *sep);

/**
 * Splits `str` using the characters in `sep` and returns the last result.
 * Creates a copy of `str` so it isn't mutated.
 *
 * Returns `NULL` when:
 * - `str_clone` fails to duplicate `str`.
 * - `__str_tok` returns a `NULL` token.
 * - `str_clone` fails to duplicate `first` (returned value).
 */
char *str_split_last(char *str, const char *sep);

/**
 * Returns `s1` and `s1` concatenated into one string.
 *
 * This function is not meant to be called directly, but by other functions in
 * the `str_concat` family.
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
 * - Any of the passed strings is `NULL`.
 * - `str_clone` or `__str_concat` fail.
 */
char *str_concat_arr(char **arr, size_t size);

/**
 * Returns a string that joins all elements in `arr` using `joiner`.
 *
 * Returns `NULL` when:
 * - `joiner`, `arr` or `size` evaluate to false.
 * - `str_clone` or `__str_concat` fail.
 */
char *str_join_arr(char *joiner, char **arr, size_t size);

/**
 * Removes characters on the left side of `s` using `isspace`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`.
 */
char *str_ltrim(char *s);

/**
 * Removes characters on the right side of `s` using `isspace`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`.
 */
char *str_rtrim(char *s);

/**
 * Calls `str_rtrim`, then `str_ltrim`.
 * Does not create a copy of `s`.
 *
 * Returns `NULL` when:
 * - `s` is `NULL`.
 */
char *str_trim(char *s);

/**
 * Returns a boolean value indicating whether `s1` is the same as `s2`.
 *
 * Returns `false` when:
 * - `s1` is `NULL`.
 * - `s2` is `NULL`.
 */
bool str_eq(char *s1, char *s2);

/**
 * `strcmp` replacement.
 *
 * Returns the lexicographical difference of `s1` and `s2`.
 *
 * It has some slight differences with `strcmp`:
 * - `strcmp` segfaults if `s1` or `s2` are NULL.
 *   `str_diff` will return -1 so you can handle this case.
 *
 * - `strcmp` and `str_diff` return different values when using the same
 *   parameters. This is because `str_diff` returns the real difference of the
 *   compared character pair.
 *
 *   For example:
 *   - `strcmp("abc", "def") == -1`
 *   - `str_diff("abc", "def") == -3`
 *
 * Returns `-1` when:
 * - `s1` is `NULL`.
 * - `s2` is `NULL`.
 */
int str_diff(char *s1, char *s2);

/**
 * Prints `s`, its `length` and whether `s` is null-terminated.
 *
 * Returns `false` when:
 * - `s` is `NULL`.
 */
bool str_info(char *s);

#endif // STR_H_

// This allows the user to use an implementation in more than one file without linker errors
#if !defined(__STRUCTYPES_STR_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION))
#define __STRUCTYPES_STR_IMPLEMENTED

#ifdef STRUCTYPES_DEBUG
static void _str_err(const char *file, int line, const char *func, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s:%d] %s: ", file, line, func);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define STR_THROW(ret, fmt, ...) ({ _str_err(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); return ret; })
#else
#define STR_THROW(ret, fmt, ...) ({ return ret; })
#endif // STRUCTYPES_DEBUG

/**
 * Concatenates all strings passed and returns the result.
 *
 * Returns `NULL` when:
 * - `str_concat_arr` fails.
 */
#define str_concat(...) \
    str_concat_arr((char *[]){ __VA_ARGS__ }, sizeof((char *[]){ __VA_ARGS__ }) / sizeof(char *))

/**
 * Returns a string that joins all elements in arr using joiner.
 *
 * Returns `NULL` when:
 * - `str_join_arr` fails.
 */
#define str_join(joiner, ...) \
    str_join_arr(joiner, (char *[]){ __VA_ARGS__ }, sizeof((char *[]){ __VA_ARGS__ }) / sizeof(char *))

size_t str_len(char *s) {
    if (!s) STR_THROW(0, "s evaluates to false");

    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char *str_clone(char *s) {
    if (!s) STR_THROW(NULL, "s evaluates to false");

    size_t len = str_len(s) + 1;
    char *copy = malloc(len);
    if (!copy) STR_THROW(NULL, "malloc error");

    for (size_t i = 0; i < len; i++) copy[i] = s[i];

    return copy;
}

bool str_free(char **s) {
    if (s == NULL || *s == NULL) return true;

    free(*s);
    *s = NULL;

    return true;
}

char str_char_at(char *str, int n) {
    int len = (int)str_len(str);

    if (abs(n) > len)
        STR_THROW('\0', "index %d out of bounds for string of size %d", n, len);

    return (n < 0) ? str[len + n] : str[n];
}

bool str_char_is_space(unsigned char c) {
    return c == ' '
    || c == '\n'
    || c == '\t'
    || c == '\r'
    || c == '\f'
    || c == '\v';
}

char *__str_tok(char **str, const char *sep) {
    char *tmp = *str;

    if (!tmp || !*tmp) return NULL;
    if (!sep) STR_THROW(NULL, "sep evaluates to false");

    char *ret = malloc(str_len(tmp) + 1);
    char *token = ret;

    while (*tmp && *tmp != *sep) *ret++ = *tmp++;
    *ret = '\0';

    if (*tmp == *sep) tmp++;

    int i = 0, j = 0;
    while (tmp[i]) tmp[j++] = tmp[i++];
    tmp[j] = '\0';

    *str = tmp;

    return token;
}

char **str_split(char *str, const char *sep, size_t *total) {
    if (!str || !sep) STR_THROW(NULL, "str or sep evaluate to false");

    char *tmp = str_clone(str);
    if (tmp == NULL) STR_THROW(NULL, "error cloning str");
    char *ptr = tmp;

    char **tokens = NULL;
    size_t size = sizeof(char *);

    *total = 0;

    char *token;
    while ((token = __str_tok(&tmp, sep))) {
        char **new_tokens = realloc(tokens, (*total + 1) * size);
        if (!new_tokens) {
            free(tokens);
            free(tmp);
            STR_THROW(NULL, "realloc error");
        }

        tokens = new_tokens;
        tokens[(*total)++] = token;
    }

    free(ptr);

    return tokens;
}

char *str_split_first(char *str, const char *sep) {
    char *tmp = str_clone(str);
    if (tmp == NULL) STR_THROW(NULL, "error cloning str");
    char *ptr = tmp;

    char *token = __str_tok(&tmp, sep);
    if (token == NULL) {
        free(tmp);
        STR_THROW(NULL, "__str_tok error");
    }

    free(ptr);

    return token;
}

char *str_split_last(char *str, const char *sep) {
    if (!str) return str;

    char *tmp = str_clone(str);
    if (tmp == NULL) STR_THROW(NULL, "error cloning str");
    char *ptr = tmp;

    char *token;
    char *last = NULL;
    while ((token = __str_tok(&tmp, sep))) {
        if (last) free(last);
        last = token;
    }

    free(ptr);

    return last;
}

char *__str_concat(char *s1, char *s2) {
    if (s1 == NULL) STR_THROW(NULL, "s1 can't be NULL");
    if (s2 == NULL) STR_THROW(NULL, "s2 can't be NULL");

    size_t len = str_len(s1);

    char *clone = str_clone(s1);
    if (!clone) STR_THROW(NULL, "str_clone error");

    char *buf = realloc(clone, len + str_len(s2) + 1);
    if (!buf) {
        free(clone);
        STR_THROW(NULL, "realloc error");
    }

    char *ptr = buf + len;

    for (const char *cur = s2; *cur; cur++) *ptr++ = *cur;
    *ptr = '\0';

    return buf;
}

char *str_concat_arr(char **arr, size_t size) {
    if (!arr) STR_THROW(NULL, "arr evaluates to false");
    if (!size) STR_THROW(NULL, "size can't be 0");

    char *buf = str_clone(arr[0]);
    if (!buf) STR_THROW(NULL, "str_clone failed");

    for (size_t i = 1; i < size; i++) {
        char *tmp = __str_concat(buf, arr[i]);
        if (!tmp) {
            free(buf);
            STR_THROW(NULL, "__str_concat error on loop");
        }

        free(buf);

        buf = tmp;
    }

    return buf;
}

char *str_join_arr(char *joiner, char **arr, size_t size) {
    if (!joiner) STR_THROW(NULL, "joiner evaluates to false");
    if (!arr) STR_THROW(NULL, "arr evaluates to false");
    if (!size) STR_THROW(NULL, "size can't be 0");

    char *buf = str_clone(arr[0]);
    if (!buf) STR_THROW(NULL, "str_clone failed");

    for (size_t i = 1; i < size; i++) {
        char *tmp = str_concat(buf, joiner, arr[i]);
        if (!tmp) {
            free(buf);
            STR_THROW(NULL, "str_concat failed to join with %s", arr[i]);
        }

        free(buf);

        buf = tmp;
    }

    return buf;
}

char *str_ltrim(char *s) {
    if (s == NULL) STR_THROW(NULL, "s can't be NULL");
    while(str_char_is_space(*s)) s++;
    return s;
}

char *str_rtrim(char *s) {
    if (s == NULL) STR_THROW(NULL, "s can't be NULL");

    int len = str_len(s);
    if (len == 0) return s;

    char *back;
    back = s + len - 1;

    while (back >= s && str_char_is_space(*back)) back--;
    *(back+1) = '\0';

    return s;
}

char *str_trim(char *s) {
    if (s == NULL) STR_THROW(NULL, "s can't be NULL");
    return str_ltrim(str_rtrim(s));
}

bool str_eq(char *s1, char *s2) {
    if (s1 == NULL) STR_THROW(false, "s1 can't be NULL");
    if (s2 == NULL) STR_THROW(false, "s2 can't be NULL");

    while (*s1 && *s2) if (*s1++ != *s2++) return false;

    return true;
}

int str_diff(char *s1, char *s2) {
    if (s1 == NULL) STR_THROW(-1, "s1 can't be NULL");
    if (s2 == NULL) STR_THROW(-1, "s2 can't be NULL");

    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;
    return (unsigned char)*s1 - (unsigned char)*s2;
}

bool str_info(char *s) {
    if (s == NULL) STR_THROW(false, "s can't be NULL");

    printf("string: %s\n", s);
    printf("len: %zu\n", str_len(s));

    return true;
}

#endif // STRUCTYPES_IMPLEMENTATION
