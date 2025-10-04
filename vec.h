// vec.h - A vector implementation that stores generic pointers.
//
// All functions return a value for error handling:
// - int: 0 on failure, 1 on success.
// - void *: NULL on failure, void * on success.
//
// Useful macros:
// - VEC_IMPLEMENTATION: implementations of the Vec structure.
// - VEC_CAPACITY_STEP: used to initialize and realloc the Vec's capacity.

#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Dynamic array that holds opaque elements. */
typedef struct Vec {
    /* Array of opaque elements.*/
    void *items;
    /* Size of a Vec item.*/
    size_t item_size;
    /* How many items the Vec is holding.*/
    size_t size;
    /* How many items the Vec can hold.*/
    size_t capacity;
} Vec;

/* Creates a new Vec. Uses `item_size` to `malloc` items. */
Vec *vec_new(size_t item_size);

/* Frees `v->items` and `v`. To free strings, use `vec_free_deep`. */
int vec_free(Vec *v);

/* Frees `v->items` (iterating over each element) and `v`. */
int vec_free_deep(Vec *v);

/* Adds an `item` at the end of `v->items`. */
int vec_push(Vec *v, void *item);

/* Adds `src->items` at the end of `dst->items`. */
int vec_extend(Vec *dst, Vec *src);

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
int vec_print(Vec *v);

#endif // VEC_H_

#ifdef VEC_IMPLEMENTATION

#ifndef VEC_CAPACITY_STEP
#define VEC_CAPACITY_STEP 4
#endif // VEC_CAPACITY_STEP

Vec *vec_new(size_t item_size) {
    if (VEC_CAPACITY_STEP < 0) {
        fprintf(stderr, "VEC_CAPACITY_STEP can't be negative");
        return NULL;
    }

    Vec *v = (Vec *)malloc(sizeof(Vec));

    if (!v) {
        fprintf(stderr, "malloc on vec_new: initialize vector");
        return NULL;
    }

    v->size = 0;
    v->capacity = VEC_CAPACITY_STEP;
    v->item_size = item_size;
    v->items = malloc(v->capacity * v->item_size);

    if (!v->items) {
        fprintf(stderr, "malloc on vec_new: initialize items");
        return NULL;
    }

    return v;
}

int vec_free(Vec *v) {
    if (!v) return 0;

    free(v->items);
    free(v);

    return 1;
}

int vec_free_deep(Vec *v) {
    if (!v) return 0;

    for (size_t i = 0; i < v->size; i++)
        free(*(void **)vec_get(v, i));

    return vec_free(v);
}

int vec_push(Vec *v, void *item) {
    if (v->size == v->capacity) {
        if (VEC_CAPACITY_STEP < 0) {
            fprintf(stderr, "VEC_CAPACITY_STEP can't be negative");
            return 0;
        }

        v->capacity += VEC_CAPACITY_STEP;

        void *tmp = realloc(v->items, v->capacity * v->item_size);
        if (!tmp) {
            fprintf(stderr, "realloc on vec_push");
            return 0;
        }

        v->items = tmp;
    }

    memcpy(
        (char *)v->items + v->size * v->item_size,
        item,
        v->item_size
    );

    v->size++;

    return 1;
}

int vec_extend(Vec *dst, Vec *src) {
    if (src->size == 0) {
        fprintf(stderr, "src vector is uninitialized");
        return 0;
    }

    if (dst->item_size != src->item_size) {
        fprintf(stderr, "Item size must be the same for both vectors");
        return 0;
    }

    size_t totalSize = dst->size + src->size;

    if (totalSize > dst->capacity) {
        void *tmp = realloc(dst->items, totalSize * dst->item_size);

        if (!tmp) {
            fprintf(stderr, "realloc on vec_extend");
            return 0;
        }

        dst->items = tmp;
    }

    memcpy(
        (char *)dst->items + dst->size * dst->item_size,
        src->items,
        src->size * dst->item_size
    );

    dst->size = totalSize;

    return 1;
}

void *vec_get(Vec *v, size_t index) {
    if (v->size == 0) {
        fprintf(stderr, "Vector is uninitialized");
        return NULL;
    }

    if (index >= v->size) {
        fprintf(stderr, "Index %zu out of bounds (size %zu)", index, v->size);
        return NULL;
    }

    return ((char *)v->items + index * v->item_size);
}

void *vec_get_ptr(Vec *v, size_t index) {
    return *(void **)vec_get(v, index);
}

int vec_print(Vec *v) {
    if (v->size == 0) {
        fprintf(stderr, "Vector is uninitialized");
        return 0;
    }

    for (size_t i = 0; i < v->size; i++)
        printf("%d %s\n", i, *(char **)vec_get(v, i));

    return 1;
}

#endif // VEC_IMPLEMENTATION
