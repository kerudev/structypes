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
// - vec_get_as(v, i, type)

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
    /* Size of a Vec item.*/
    size_t offset;
    /* How many items the Vec is holding.*/
    size_t size;
    /* How many items the Vec can hold.*/
    size_t capacity;
} Vec;

/**
 * Creates a new `Vec`. Uses `offset` to `malloc` items.
 *
 * Returns `NULL` when:
 * - `capacity < 1`.
 * - `VEC_CAPACITY_STEP < 1`.
 * - `malloc` fails to allocate a new `Vec`.
 * - `malloc` fails to initialize `Vec.items`.
 */
Vec *vec_new(size_t offset);

/**
 * Frees `v->items` (iterating over each element) and `v`.
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
 * - `dst->offset != src->offset`.
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
 * Prints each item in `v`.
 *
 * Returns `false` when:
 * - `v->size == 0`.
 */
bool vec_print(Vec *v);

/**
 * Prints the values of `offset`, `size` and `capacity` of `v`.
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

/**
 * Returns the value at `i` casted to `type`.
 * If `vec_get` returns `NULL`, it won't be cast to prevent undefined behaviour.
 */
#define vec_get_as(v, i, type) ({   \
    void *_tmp = vec_get((v), (i)); \
    (_tmp) ? (type)_tmp : NULL;  \
})

Vec *vec_new(size_t offset) {
    if (offset < 1)
        THROW(NULL, "vec_new: offset must be 1 or greater");

    if (VEC_CAPACITY_STEP < 1)
        THROW(NULL, "vec_new: VEC_CAPACITY_STEP must be 1 or greater");

    Vec *v = malloc(sizeof(Vec));
    if (!v)
        THROW(NULL, "vec_new: malloc error on initialize vector");

    v->size = 0;
    v->capacity = VEC_CAPACITY_STEP;
    v->offset = offset;
    v->items = malloc(v->capacity * v->offset);

    if (!v->items)
        THROW(NULL, "vec_new: malloc error on initialize items");

    return v;
}

bool vec_free(Vec *v) {
    if (!v) THROW(false, "vec_free: v evaluates to false");

    if (v->items) {
        for (size_t i = 0; i < v->size; i++) free(v->items[i]);
        free(v->items);
    }

    free(v);

    return true;
}

bool vec_push(Vec *v, void *item) {
    if (!v) THROW(false, "vec_push: v evaluates to false");

    if (v->size == v->capacity) {
        if (VEC_CAPACITY_STEP < 1)
            THROW(false, "vec_push: VEC_CAPACITY_STEP must be 1 or greater");

        v->capacity += VEC_CAPACITY_STEP;

        void *tmp = realloc(v->items, v->capacity * v->offset);
        if (!tmp) THROW(false, "vec_push: realloc error");

        v->items = tmp;
    }

    v->items[v->size++] = item;

    return true;
}

// TODO refactor
bool vec_extend(Vec *dst, Vec *src) {
    if (!dst) THROW(false, "vec_extend: dst evaluates to false");

    if (!src->size)
        THROW(false, "vec_extend: src vector is uninitialized");

    if (dst->offset != src->offset)
        THROW(false, "vec_extend: offset doesn't match on both vectors");

    size_t totalSize = dst->size + src->size;

    if (totalSize > dst->capacity) {
        void *tmp = realloc(dst->items, totalSize * dst->offset);
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

    printf("offset:   %zu\n", v->offset);
    printf("size:     %zu\n", v->size);
    printf("capacity: %zu\n", v->capacity);

    return true;
}

#endif // VEC_IMPLEMENTATION
