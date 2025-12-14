// vec.h - A vector implementation that stores generic pointers.
//
// Library macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
//
// File macros:
// - VEC_IMPLEMENTATION: implementations of the Vec structure.
// - VEC_CAPACITY_STEP: used to initialize and realloc the Vec's capacity.
//   By default, `4`.
//
// Function macros:
// - vec_from(arr)

#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/* Dynamic array that holds generic pointers. */
typedef struct Vec {
    /* Array of generic pointers.*/
    void **items;
    /* How many items the Vec is holding.*/
    size_t size;
    /* How many items the Vec can hold.*/
    size_t capacity;
} Vec;

/**
 * Creates a new `Vec`.
 *
 * Returns `NULL` when:
 * - `VEC_CAPACITY_STEP < 1`.
 * - `malloc` fails to allocate a new `Vec`.
 * - `malloc` fails to initialize `Vec.items`.
 */
Vec *vec_new();

/**
 * Creates a new `Vec` from an existing array and `size`.
 *
 * Return `NULL` when:
 * - `arr` evaluates to false.
 * - `vec_new` or `vec_push` fail.
 */
Vec *vec_from_arr(void **arr, size_t size);

/**
 * Frees `v->items` and `v`.
 * If you need to free each element, use `vec_free` instead.
 *
 * Returns `false` when:
 * - `v` evaluates to false.
 */
bool vec_free_struct(Vec *v);

/**
 * Frees `v->items` (iterating over each element) and `v`.
 * Uses `free`, so use this to free data like `char*` or `void*`.
 *
 * Returns `false` when:
 * - `v` evaluates to false.
 */
bool vec_free(Vec *v);

/**
 * Adds an `item` at the end of `v->items`.
 *
 * Returns `false` when:
 * - `VEC_CAPACITY_STEP < 1`.
 * - `realloc` for `v->items` fails.
 */
bool vec_push(Vec *v, void *item);

/**
 * Adds `src->items` at the end of `dst->items`.
 *
 * Returns `false` when:
 * - `size->size == 0`.
 * - `realloc` for `dst->items` fails.
 */
bool vec_extend(Vec *dst, Vec *src);

/**
 * Returns a pointer to the element at `n`. Supports negative indexing.
 *
 * Returns `NULL` when:
 * - `n` is out of bounds.
 * - `v->size == 0`.
 */
void *vec_get(Vec *v, int n);

/**
 * Returns the value at the last element of `v` and assigns it to `NULL`.
 *
 * Returns `NULL` when:
 * - `v` evaluates to false.
 * - `v->size == 0`.
 */
void *vec_pop(Vec *v);

/**
 * Sorts the elements of `v->items`.
 *
 * Returns `false` when:
 * - `v` evaluates to false.
 */
bool vec_sort(Vec *v);

/**
 * Compares `v1->items` with `v2->items`.
 *
 * Both arrays must be ordered to be considered equal (see `vec_sort`).
 *
 * Returns `false` when:
 * - `v1` or `v2` evaluate to false.
 * - `v1->size` or `v2->size` are different.
 * - The compared elements are different.
 */
bool vec_eq(Vec *v1, Vec *v2);

/**
 * Prints each item in `v`.
 *
 * Returns `false` when:
 * - `v->size == 0`.
 */
bool vec_print(Vec *v);

/**
 * Prints the values of `size` and `capacity` of `v`.
 *
 * Returns `false` when:
 * - `v` evaluates to false.
 */
bool vec_info(Vec *v);

#endif // VEC_H_

#if !defined(__VEC_IS_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(VEC_IMPLEMENTATION))
#define __VEC_IS_IMPLEMENTED

#ifndef VEC_CAPACITY_STEP
#define VEC_CAPACITY_STEP 4
#endif // VEC_CAPACITY_STEP

#ifdef STRUCTYPES_DEBUG
static void _vec_err(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define THROW(ret, msg, ...) ({ _vec_err(msg, ##__VA_ARGS__); return ret; })
#else
#define THROW(ret, msg, ...) ({ return ret; })
#endif

static int _vec_comp(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;

    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;

    int diff = (unsigned char)*s1 - (unsigned char)*s2;

    return diff;
}

/**
 * Creates a new `Vec` from an existing buffer.
 * This is a conveniency macro that calculates the size of `arr`.
 *
 * Return `NULL` when:
 * - `arr` evaluates to false.
 * - `vec_new` or `vec_push` fail.
 */
#define vec_from(arr) \
    vec_from_arr((void**)arr, sizeof(arr)/sizeof(arr[0]))

Vec *vec_new() {
    if (VEC_CAPACITY_STEP < 1)
        THROW(NULL, "vec_new: VEC_CAPACITY_STEP must be 1 or greater");

    Vec *v = malloc(sizeof(Vec));
    if (!v)
        THROW(NULL, "vec_new: malloc error on initialize vector");

    v->size = 0;
    v->capacity = VEC_CAPACITY_STEP;
    v->items = malloc(v->capacity * sizeof(void *));

    if (!v->items)
        THROW(NULL, "vec_new: malloc error on initialize items");

    return v;
}

Vec *vec_from_arr(void **arr, size_t size) {
    if (!arr) THROW(NULL, "vec_from_arr: arr evaluates to false");

    Vec *v = vec_new();
    if (!v) THROW(NULL, "vec_from_arr: vec_new failed");

    for (size_t i = 0; i < size; i++)
        if (!vec_push(v, arr[i])) THROW(NULL, "vec_push: vec_new failed");

    return v;
}

bool vec_free_struct(Vec *v) {
    if (!v) THROW(false, "vec_free_struct: v evaluates to false");

    free(v->items);
    free(v);
    v = NULL;

    return true;
}

bool vec_free(Vec *v) {
    if (!v) THROW(false, "vec_free: v evaluates to false");

    if (v->items) {
        for (size_t i = 0; i < v->size; i++)
            if (v->items[i]) free(v->items[i]);

        free(v->items);
        v->items = NULL;
    }

    free(v);
    v = NULL;

    return true;
}

bool vec_push(Vec *v, void *item) {
    if (!v) THROW(false, "vec_push: v evaluates to false");

    if (v->size == v->capacity) {
        if (VEC_CAPACITY_STEP < 1)
            THROW(false, "vec_push: VEC_CAPACITY_STEP must be 1 or greater");

        v->capacity += VEC_CAPACITY_STEP;

        void *tmp = realloc(v->items, v->capacity * sizeof(void *));
        if (!tmp) THROW(false, "vec_push: realloc error");

        v->items = tmp;
    }

    v->items[v->size++] = item;

    return true;
}

bool vec_extend(Vec *dst, Vec *src) {
    if (!dst) THROW(false, "vec_extend: dst evaluates to false");

    if (!src->size)
        THROW(false, "vec_extend: src vector is uninitialized");

    size_t totalSize = dst->size + src->size;

    if (totalSize > dst->capacity) {
        void *tmp = realloc(dst->items, totalSize * sizeof(void *));
        if (!tmp) THROW(false, "vec_extend: realloc error");

        dst->items = tmp;
        dst->capacity = totalSize;
    }

    for (size_t i = 0; i < src->size; i++)
        dst->items[dst->size + i] = src->items[i];

    dst->size = totalSize;

    return true;
}

void *vec_get(Vec *v, int n) {
    if (!v) THROW(false, "vec_get: v evaluates to false");
    if (!v->size) THROW(NULL, "vec_get: vector is uninitialized");

    int len = (int)v->size;

    if (abs(n) > len)
        THROW(NULL, "vec_get: index %d out of bounds (size %d)", n, len);

    return (n < 0) ? v->items[n + len] : v->items[n];
}

void *vec_pop(Vec *v) {
    if (!v) THROW(NULL, "vec_pop: v evaluates to false");
    if (!v->size) THROW(NULL, "vec_pop: v has no elements");

    v->size--;

    void *value = v->items[v->size];
    v->items[v->size] = NULL;

    return value;
}

bool vec_sort(Vec *v) {
    if (!v) THROW(false, "vec_sort: v evaluates to false");

    qsort(v->items, v->size, sizeof(void *), _vec_comp);

    return true;
}

bool vec_eq(Vec *v1, Vec *v2) {
    if (!v1) THROW(false, "vec_eq: v1 evaluates to false");
    if (!v2) THROW(false, "vec_eq: v2 evaluates to false");

    if (v1->size != v2->size) THROW(false, "vec_eq: sizes are different");

    for (size_t i = 0; i < v1->size; i++) {
        char *s1 = v1->items[i];
        char *s2 = v2->items[i];

        while (*s1 && *s2) if (*s1++ != *s2++) return false;
    }

    return true;
}

bool vec_print(Vec *v) {
    if (!v) THROW(false, "vec_print: v evaluates to false");

    if (!v->size)
        THROW(false, "vec_print: vector is uninitialized");

    for (size_t i = 0; i < v->size; i++)
        printf("%zu %s\n", i, (char *)vec_get(v, i));

    return true;
}

bool vec_info(Vec *v) {
    if (!v) THROW(false, "vec_info: v evaluates to false");

    printf("size:     %zu\n", v->size);
    printf("capacity: %zu\n", v->capacity);

    return true;
}

#endif // VEC_IMPLEMENTATION
