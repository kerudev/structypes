// hashmap.c - A hashmap implementation. Uses closed addressing.
//
// Library macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
//
// File macros:
// - HASHMAP_IMPLEMENTATION: implementations of node functions.
// - HASHMAP_CAPACITY_STEP: how much the capacity will increment on resize.
//   By default, `1028`.
// - HASHMAP_LOAD_FACTOR: when `hm->size / hm->capacity > HASHMAP_LOAD_FACTOR`,
//   `hm` will be resized. By default, `0.75`.
// - HASHMAP_HASH_ALGORITHM: reference of the hashing algorithm.
//   By default, `djb2`.
//
// Function macros:
// - hashmap_hash

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct {
    char *key;
    void *value;
} KV;

typedef struct {
    KV **items;
    size_t size;
    size_t capacity;
} HashMap;

/**
 * Creates a new `HashMap` with `capacity`.
 *
 * Returns `NULL` when:
 * - `capacity < 1`.
 * - `HASHMAP_CAPACITY_STEP < 1`.
 * - `HASHMAP_LOAD_FACTOR < 0 || 1 < HASHMAP_LOAD_FACTOR`.
 * - `malloc` fails to allocate a new `HashMap`.
 */
HashMap *hashmap_new(size_t capacity);

/**
 * Creates a new `KV`.
 *
 * Returns `NULL` when:
 * - `k` is `NULL`.
 * - `malloc` fails to allocate a new `KV`.
 */
KV *hashmap_new_kv(char *k, void *v);

/**
 * Frees a `HashMap` and its `KV`.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 * - `hashmap_free_kv` fails.
 */
bool hashmap_free(HashMap *hm);

/**
 * Frees a `KV`.
 *
 * Returns `false` when:
 * - `kv` evaluates to false.
 */
bool hashmap_free_kv(KV *kv);

/**
 * Returns the value stored under `k`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->size` is 0.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `k` is `NULL`.
 */
void *hashmap_get(HashMap *hm, char *k);

/**
 * Creates a new `KV` using `k` and `v`, and saves it into `hm->items`.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `k` is `NULL`.
 * - `hashmap_resize` fails (called when the load factor is surpassed).
 */
bool hashmap_set(HashMap *hm, char *k, void *v);

/**
 * Creates a new `HashMap` with `capacity` and rehashes all pairs.
 *
 * Returns `false` when:
 * - `capacity` is 0.
 * - `hm` evaluates to false.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `hm->size > capacity`.
 * - `hashmap_new` or `hashmap_set` fail.
 */
bool hashmap_resize(HashMap *hm, size_t capacity);

#endif // HASHMAP_H_

#if !defined(__HASHMAP_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(HASHMAP_IMPLEMENTATION))
#define __HASHMAP_IMPLEMENTED

#ifndef HASHMAP_CAPACITY_STEP
#define HASHMAP_CAPACITY_STEP 1024
#endif // HASHMAP_CAPACITY_STEP

#ifndef HASHMAP_LOAD_FACTOR
#define HASHMAP_LOAD_FACTOR 0.75
#endif // HASHMAP_LOAD_FACTOR

#ifndef HASHMAP_HASH_ALGORITHM
#define HASHMAP_HASH_ALGORITHM djb2
#endif // HASHMAP_HASH_ALGORITHM

#ifdef STRUCTYPES_DEBUG
static void _hashmap_err(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define THROW(ret, msg, ...) ({ _hashmap_err(msg, ##__VA_ARGS__); return ret; })
#else
#define THROW(ret, msg, ...) ({ return ret; })
#endif

/**
 * Hashes a key using `HASHMAP_HASH_ALGORITHM`, then takes the modulo of `capacity`.
 */
#define hashmap_hash(k, capacity) \
    HASHMAP_HASH_ALGORITHM((unsigned char *)k) % capacity;

unsigned long djb2(unsigned char *str) {
    unsigned long hash = 5381;

    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

HashMap *hashmap_new(size_t capacity) {
    if (capacity < 1)
        THROW(NULL, "hashmap_new: capacity must be 1 or greater");

    HashMap *hm = malloc(sizeof(HashMap));
    if (!hm)
        THROW(NULL, "hashmap_new: malloc error on initialize hashmap");

    if (HASHMAP_CAPACITY_STEP < 1)
        THROW(NULL, "hashmap_new: HASHMAP_CAPACITY_STEP must be 1 or greater");

    if (HASHMAP_LOAD_FACTOR < 0 || 1 < HASHMAP_LOAD_FACTOR)
        THROW(NULL, "hashmap_new: HASHMAP_LOAD_FACTOR must be between 0 and 1");

    hm->size = 0;
    hm->capacity = capacity;
    hm->items = malloc(hm->capacity * sizeof(KV *));

    return hm;
}

KV *hashmap_new_kv(char *k, void *v) {
    if (k == NULL)
        THROW(NULL, "hashmap_new_kv: k can't be NULL");

    KV *kv = malloc(sizeof(KV));
    if (!kv) THROW(NULL, "hashmap_new_kv: malloc error on initialize kv");

    kv->key = k;
    kv->value = v;

    return kv;
}

bool hashmap_free(HashMap *hm) {
    if (!hm) THROW(false, "hashmap_free: hm evaluates to false");

    for (size_t i = 0; i < hm->capacity; i++) {
        if (!hm->size) break;
        if (!hm->items[i]) continue;

        if (!hashmap_free_kv(hm->items[i]))
            THROW(false, "hashmap_free: error freeing kv");

        hm->size--;
    }

    // TODO fix memory leak
    // free(hm->items);

    free(hm);

    return true;
}

bool hashmap_free_kv(KV *kv) {
    if (!kv) THROW(false, "hashmap_free_kv: kv evaluates to false");

    if (kv->value) free(kv->value);

    free(kv->key);
    free(kv);

    return true;
}

void *hashmap_get(HashMap *hm, char *k) {
    if (!hm) THROW(NULL, "hashmap_get: hm evaluates to false");
    if (!hm->size) THROW(NULL, "hashmap_get: hashmap is uninitialized");
    if (!hm->capacity) THROW(NULL, "hashmap_get: capacity can't be 0");
    if (!hm->items) THROW(NULL, "hashmap_get: items evaluates to false");
    if (k == NULL) THROW(NULL, "hashmap_get: k can't be null");

    unsigned long hash = hashmap_hash(k, hm->capacity);

    return hm->items[hash]->value;
}

bool hashmap_set(HashMap *hm, char *k, void *v) {
    if (!hm) THROW(false, "hashmap_set: hm evaluates to false");
    if (!hm->capacity) THROW(false, "hashmap_set: capacity can't be 0");
    if (!hm->items) THROW(false, "hashmap_set: items evaluates to false");
    if (k == NULL) THROW(NULL, "hashmap_set: k can't be null");

    if (hm->size / hm->capacity >= HASHMAP_LOAD_FACTOR)
        if (!hashmap_resize(hm, hm->capacity + HASHMAP_CAPACITY_STEP))
            THROW(false, "hashmap_set: resize error");

    unsigned long hash = hashmap_hash(k, hm->capacity);
    hm->items[hash] = hashmap_new_kv(k, v);

    hm->size++;

    return true;
}

bool hashmap_resize(HashMap *hm, size_t capacity) {
    if (!capacity) THROW(false, "hashmap_resize: capacity can't be 0");

    if (!hm) THROW(false, "hashmap_resize: hm evaluates to false");
    if (!hm->capacity) THROW(false, "hashmap_resize: capacity can't be 0");
    if (!hm->items) THROW(false, "hashmap_resize: items evaluates to false");

    if (hm->size > capacity)
        THROW(false, "hashmap_resize: new capacity can't be less than current size");

    HashMap *tmp = hashmap_new(capacity);
    if (!tmp) THROW(false, "hashmap_resize: error creating new HashMap");

    if (hm->size) {
        for (size_t i = 0; i < hm->capacity; i++) {
            if (!hm->items[i]) continue;

            if (!hashmap_set(tmp, hm->items[i]->key, hm->items[i]->value))
                THROW(false, "hashmap_resize: rehash fail");
        }
    }

    free(hm->items);

    hm->items = tmp->items;
    hm->capacity = tmp->capacity;
    hm->size = tmp->size;

    free(tmp);

    return true;
}

#endif // HASHMAP_IMPLEMENTATION
