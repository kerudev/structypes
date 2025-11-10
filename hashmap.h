// hashmap.c - A hashmap implementation. Uses closed addressing.
//
// Library macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
// - HASHMAP_IMPLEMENTATION: implementations of node functions.
//
// File macros:
// - HASHMAP_CAPACITY_STEP: 
// - HASHMAP_LOAD_FACTOR: 
// - HASHMAP_HASH_FUNC: 
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
} HM_KV;

typedef struct {
    HM_KV *items;
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
 * Creates a new `HM_KV`.
 *
 * Returns `NULL` when:
 * - `k` is `NULL`.
 * - `malloc` fails to allocate a new `HM_KV`.
 */
HM_KV *hashmap_new_kv(char *k, void *v);

/**
 * Returns a `HM_KV` based on the hash of `k`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->items` evaluates to false.
 * - `hm->size` is 0.
 */
HM_KV *hashmap_get(HashMap *hm, char *k);

/**
 * Creates a new `HM_KV` using `k` and `v`, and saves it into `hm->items`.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 * - `hm->items` evaluates to false.
 */
bool hashmap_set(HashMap *hm, char *k, void *v);

/**
 * Saves `kv` into `hm->items`.
 *
 * Returns `false` when:
 * - `kv` evaluates to false.
 * - `hashmap_set` fails.
 */
bool hashmap_set_kv(HashMap *hm, HM_KV *kv);

/**
 * Creates a new `HashMap` with `capacity` and rehashes all pairs.
 *
 * Returns `false` when:
 * - `hm` or `hm->items` evaluate to false. 
 * - `hm->size` is 0.
 * - `capacity` is 0
 * - `hashmap_new` or `hashmap_set_kv` fail.
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

#ifndef HASHMAP_HASH_FUNC
#define HASHMAP_HASH_FUNC djb2
#endif // HASHMAP_HASH_FUNC

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
 * Hashes a key using `HASHMAP_HASH_FUNC`, then takes the modulo of `capacity`.
 */
#define hashmap_hash(k, capacity) \
    HASHMAP_HASH_FUNC((unsigned char *)k) % capacity;

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

    hm->items = NULL;
    hm->size = 0;
    hm->capacity = capacity;

    return hm;
}

HM_KV *hashmap_new_kv(char *k, void *v) {
    if (k == NULL)
        THROW(NULL, "hashmap_new_kv: k can't be NULL");

    HM_KV *kv = malloc(sizeof(HM_KV));
    if (!kv) THROW(NULL, "hashmap_new_kv: malloc error on initialize kv");

    kv->key = k;
    kv->value = v;

    return kv;
}

HM_KV *hashmap_get(HashMap *hm, char *k) {
    if (!hm) THROW(NULL, "hashmap_get: hm evaluates to false");
    if (!hm->items) THROW(NULL, "hashmap_get: items evaluates to false");
    if (!hm->size) THROW(NULL, "hashmap_get: hashmap is uninitialized");

    unsigned long hash = hashmap_hash(k, hm->capacity);
    return ((void**)hm->items)[hash];
}

bool hashmap_set(HashMap *hm, char *k, void *v) {
    if (!hm) THROW(false, "hashmap_set: hm evaluates to false");
    if (!hm->items) THROW(false, "hashmap_set: items evaluates to false");

    if (hm->size / hm->capacity > HASHMAP_LOAD_FACTOR)
        if (!hashmap_resize(hm, hm->capacity + HASHMAP_CAPACITY_STEP))
            THROW(false, "hashmap_set: resize error");

    unsigned long hash = hashmap_hash(k, hm->capacity);
    ((void**)hm->items)[hash] = hashmap_new_kv(k, v);

    return true;
}

bool hashmap_set_kv(HashMap *hm, HM_KV *kv) {
    if (!kv) THROW(false, "hashmap_set_kv: kv evaluates to false");
    return hashmap_set(hm, kv->key, kv->value);
}

bool hashmap_resize(HashMap *hm, size_t capacity) {
    if (!hm) THROW(false, "hashmap_resize: hm evaluates to false");
    if (!hm->items) THROW(false, "hashmap_resize: items evaluates to false");
    if (!hm->size) THROW(false, "hashmap_resize: hashmap is uninitialized");
    if (!capacity) THROW(false, "hashmap_resize: capacity can't be 0");

    HashMap *tmp = hashmap_new(capacity);
    if (!tmp) THROW(false, "hashmap_resize: error creating new HashMap");

    void **array = (void**)hm->items;

    for (size_t i = 0; i < hm->capacity; i++) {
        HM_KV *kv = array[i];
        if (!kv) continue;

        if (!hashmap_set_kv(tmp, kv))
            THROW(false, "hashmap_resize: rehash fail");
    }

    free(hm->items);

    hm->items = tmp->items;
    hm->capacity = tmp->capacity;

    free(tmp);

    return true;
}

#endif // HASHMAP_IMPLEMENTATION
