#ifndef VEC_H_
#define VEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Used to initialize and realloc the Vec's capacity
#ifndef VEC_INITIAL_CAP 
#define VEC_INITIAL_CAP 4
#endif // VEC_INITIAL_CAP 

/* Dynamic array that holds opaque elements */
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

/* Frees `v->items` and `v`. */
void vec_free(Vec *v);

/* Adds an `item` at the end of `v->items`. */
int vec_push(Vec *v, void *item);

/* Adds `src->items` at the end of `dst->items`. */
int vec_extend(Vec *dst, Vec *src);

/* Returns an item (NULL if out of bounds) using `index`. */
void *vec_get(Vec *v, size_t index);

#endif // VEC_H_

// Implementations of the Vec structure.
#ifdef VEC_IMPLEMENTATION

Vec *vec_new(size_t item_size) {
    Vec *v = malloc(sizeof(Vec));

    v->size = 0;
    v->capacity = VEC_INITIAL_CAP;
    v->item_size = item_size;
    v->items = malloc(v->capacity * v->item_size);

    if (!v->items) {
        perror("malloc on vec_new");
        return NULL;
    }

    return v;
}

void vec_free(Vec *v) {
    free(v->items);
    free(v);
}

int vec_push(Vec *v, void *item) {
    if (v->size == v->capacity) {
        v->capacity += VEC_INITIAL_CAP;

        void *tmp = realloc(v->items, v->capacity * v->item_size);     
        if (!tmp) {
            perror("realloc on vec_push");
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
    if (dst->item_size != src->item_size) {
        perror("Item size must be the same for both vectors");
        return 0;
    }

    size_t totalSize = dst->size + src->size;
    
    if (totalSize > dst->capacity) {
        size_t newCapacity = (dst->capacity + src->size);
        void *tmp = realloc(dst->items, newCapacity * dst->item_size);

        if (!tmp) {
            perror("realloc on vec_extend");
            return 0;
        }

        dst->items = tmp;
        dst->capacity = newCapacity;
    }

    memcpy(
        (char *)dst->items + src->size * dst->item_size,
        (char *)src->items,
        src->size * dst->item_size
    );

    dst->size = totalSize;

    return 1;
}

void *vec_get(Vec *v, size_t index) {
    if (index >= v->size) {
        fprintf(stderr, "Index %zu out of bounds (size %zu)", index, v->size);
        return NULL;
    }

    return v->items + index * v->item_size;
}

#endif // VEC_IMPLEMENTATION
