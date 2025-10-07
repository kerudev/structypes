// vec.h - A vector implementation that stores generic pointers.
//
// All functions return a value for error handling:
// - int: 0 on failure, 1 on success.
// - void *: NULL on failure, void * on success.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - VEC_IMPLEMENTATION: implementations of the Vec structure.
// - VEC_CAPACITY_STEP: used to initialize and realloc the Vec's capacity.

#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

/* Dynamic array that holds opaque elements. */
typedef struct Vec {
    /* Array of opaque elements.*/
    void *items;
    /* Size of a Vec item.*/
    size_t offset;
    /* How many items the Vec is holding.*/
    size_t size;
    /* How many items the Vec can hold.*/
    size_t capacity;
} Vec;

/* Creates a new Vec. Uses `offset` to `malloc` items. */
Vec *vec_new(size_t offset);

/* Frees `v->items` and `v`. To free strings, use `vec_free_deep`. */
bool vec_free(Vec *v);

/* Frees `v->items` (iterating over each element) and `v`. */
bool vec_free_deep(Vec *v);

/* Adds an `item` at the end of `v->items`. */
bool vec_push(Vec *v, void *item);

/* Adds `src->items` at the end of `dst->items`. */
bool vec_extend(Vec *dst, Vec *src);

/**
 * Returns a pointer to the element at `index`.
 * Returns `NULL` if `index` is out of bounds or `v->size == 0`.
 */
void *vec_get(Vec *v, size_t index);

/**
 * Returns a pointer to the element's value at `index`.
 * Returns `NULL` if `index` is out of bounds or `v->size == 0`.
 */
void *vec_get_ptr(Vec *v, size_t index);

/**
 * Returns the value at `i` casted to `type`.
 * If `vec_get` returns `NULL`, casting it may cause undefined behaviour.
 */
#define vec_get_as(v, i, type) (*(type *)vec_get((v), (i)))

/* Prints each item in `v`. */
bool vec_print(Vec *v);

#endif // VEC_H_

#if !defined(__VEC_IS_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(VEC_IMPLEMENTATION))
#define __VEC_IS_IMPLEMENTED

#ifndef VEC_CAPACITY_STEP
#define VEC_CAPACITY_STEP 4
#endif // VEC_CAPACITY_STEP

void err(char *func, char *msg, ...) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "%s: ", func);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

Vec *vec_new(size_t offset) {
    if (VEC_CAPACITY_STEP < 0) {
        err("vec_new", "VEC_CAPACITY_STEP can't be negative");
        return NULL;
    }

    Vec *v = (Vec *)malloc(sizeof(Vec));

    if (!v) {
        err("vec_new", "malloc error on initialize vector");
        return NULL;
    }

    v->size = 0;
    v->capacity = VEC_CAPACITY_STEP;
    v->offset = offset;
    v->items = malloc(v->capacity * v->offset);

    if (!v->items) {
        err("vec_new", "malloc error on initialize items");
        return NULL;
    }

    return v;
}

bool vec_free(Vec *v) {
    if (!v) return false;

    free(v->items);
    free(v);

    return true;
}

bool vec_free_deep(Vec *v) {
    if (!v) return false;

    for (size_t i = 0; i < v->size; i++)
        free(*(void **)vec_get(v, i));

    return vec_free(v);
}

bool vec_push(Vec *v, void *item) {
    if (v->size == v->capacity) {
        if (VEC_CAPACITY_STEP < 0) {
            err("vec_push", "VEC_CAPACITY_STEP can't be negative");
            return false;
        }

        v->capacity += VEC_CAPACITY_STEP;

        void *tmp = realloc(v->items, v->capacity * v->offset);
        if (!tmp) {
            err("vec_push", "realloc");
            return false;
        }

        v->items = tmp;
    }

    memcpy(
        (char *)v->items + v->size * v->offset,
        item,
        v->offset
    );

    v->size++;

    return true;
}

bool vec_extend(Vec *dst, Vec *src) {
    if (src->size == 0) {
        err("vec_extend", "src vector is uninitialized");
        return false;
    }

    if (dst->offset != src->offset) {
        err("vec_extend", "offset doesn't match on both vectors");
        return false;
    }

    size_t totalSize = dst->size + src->size;

    if (totalSize > dst->capacity) {
        void *tmp = realloc(dst->items, totalSize * dst->offset);

        if (!tmp) {
            err("vec_extend", "realloc");
            return false;
        }

        dst->items = tmp;
    }

    memcpy(
        (char *)dst->items + dst->size * dst->offset,
        src->items,
        src->size * dst->offset
    );

    dst->size = totalSize;

    return true;
}

void *vec_get(Vec *v, size_t index) {
    if (v->size == 0) {
        err("vec_get", "vector is uninitialized");
        return NULL;
    }

    if (index >= v->size) {
        err("vec_get", "index %zu out of bounds (size %zu)", index, v->size);
        return NULL;
    }

    // TODO support for negative indexing

    return ((char *)v->items + index * v->offset);
}

void *vec_get_ptr(Vec *v, size_t index) {
    return *(void **)vec_get(v, index);
}

bool vec_print(Vec *v) {
    if (v->size == 0) {
        err("vec_print", "vector is uninitialized");
        return false;
    }

    for (size_t i = 0; i < v->size; i++)
        printf("%d %s\n", i, *(char **)vec_get(v, i));

    return true;
}

#endif // VEC_IMPLEMENTATION
