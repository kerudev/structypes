// hashmap.c - A hashmap implementation. Uses closed addressing.
//
// Library macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
//
// File macros:
// - HASHMAP_IMPLEMENTATION: implementations of node functions.
// - HASHMAP_CAPACITY_STEP: how much the capacity will increment on resize.
//   By default, `1024`.
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
#include <limits.h>

typedef struct {
    char *key;
    void *value;
} KV;

typedef struct {
    KV **items;
    size_t size;
    size_t capacity;
} HashMap;

typedef unsigned long hash_t;

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
 * Frees `hm->items` and `hm`.
 *
 * To free each item inside `hm->items`, use `hashmap_free` for heap allocated
 * items and `hashmap_free_stack` for stack allocated items.
 *
 * Retruns `false` when:
 * - `hm` evaluates to false.
 */
bool hashmap_free_struct(HashMap *hm);

/**
 * Frees `kv`.
 *
 * Retruns `false` when:
 * - `kv` evaluates to false.
 */
bool hashmap_free_kv_struct(KV *kv);

/**
 * Frees each element inside `hm->items`, then calls `hashmap_free_struct`.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 * - `hashmap_free_kv` fails.
 * - `hashmap_free_struct` fails.
 */
bool hashmap_free(HashMap *hm);

/**
 * Frees `kv->key` and `kv->value`, then calls `hashmap_free_kv_struct`.
 *
 * Returns `false` when:
 * - `kv` evaluates to false.
 * - `hashmap_free_kv_struct` fails.
 */
bool hashmap_free_kv(KV *kv);

/**
 * Frees each element inside `hm->items`, then calls `hashmap_free_struct`.
 * This assumes that every `KV` has its key and value allocated in the stack.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 * - `hashmap_free_struct` fails.
 */
bool hashmap_free_stack(HashMap *hm);

/**
 * Returns the value stored under `k`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->size` is 0.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `k` is `NULL`.
 * - There is no `KV` to get a value from.
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

/**
 * Returns all `kv->key` in `hm->items`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->size` is 0.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `malloc` fails to allocate a new `char **`.
 */
char **hashmap_keys(HashMap *hm);

/**
 * Returns all `kv->value` in `hm->items`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->size` is 0.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `malloc` fails to allocate a new `void **`.
 */
void **hashmap_values(HashMap *hm);

/**
 * Returns all `kv` in `hm->items`.
 *
 * Returns `NULL` when:
 * - `hm` evaluates to false.
 * - `hm->size` is 0.
 * - `hm->capacity` is 0.
 * - `hm->items` evaluates to false.
 * - `malloc` fails to allocate a new `KV **`.
 */
KV **hashmap_items(HashMap *hm);

/**
 * Compares `hm1->items` and `hm2->items`.
 *
 * Returns `false` when:
 * - `hm1` or `hm2` evaluate to false.
 * - `hm1->size` or `hm2->size` are different.
 * - `hashmap_eq_kv` returns `false`.
 * - The compared elements are different.
 */
bool hashmap_eq(HashMap *hm1, HashMap *hm2);

// TODO assumes that kv->value is a char*
/**
 * Compares `kv1->key` with `kv2->key`, and `kv1->value` with `kv2->value`.
 *
 * Returns `false` when:
 * - `kv1` or `kv2` evaluate to false.
 * - `hashmap_eq_kv` returns `false`.
 */
bool hashmap_eq_kv(KV *kv1, KV *kv2);

/**
 * Prints each `kv` in pairs.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 */
bool hashmap_print(HashMap *hm);

/**
 * Prints the `key` and `value` from `kv`.
 *
 * Returns `false` when:
 * - `kv` evaluates to false.
 */
bool hashmap_print_kv(KV *kv);

/**
 * Prints the `size` and `capacity` from `hm`.
 *
 * Returns `false` when:
 * - `hm` evaluates to false.
 */
bool hashmap_info(HashMap *hm);

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
static void _hashmap_err(const char *file, int line, const char *func, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s:%d] %s: ", file, line, func);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define HM_THROW(ret, fmt, ...) ({ _hashmap_err(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); return ret; })
#else
#define HM_THROW(ret, fmt, ...) ({ return ret; })
#endif // STRUCTYPES_DEBUG

// TODO put this inside the HashMap struct. Having more than one hashmap would break this
static hash_t __hashmap_first_hash = INT_MAX;
static hash_t __hashmap_last_hash = 0;

/**
 * Hashes a key using `HASHMAP_HASH_ALGORITHM`, then takes the modulo of `capacity`.
 */
#define hashmap_hash(k, capacity) \
    HASHMAP_HASH_ALGORITHM((unsigned char *)k) % capacity

hash_t djb2(unsigned char *str) {
    hash_t hash = 5381;

    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

HashMap *hashmap_new(size_t capacity) {
    if (capacity < 1) HM_THROW(NULL, "capacity must be 1 or greater");

    HashMap *hm = malloc(sizeof(HashMap));
    if (!hm) HM_THROW(NULL, "malloc error");

    if (HASHMAP_CAPACITY_STEP < 1)
        HM_THROW(NULL, "HASHMAP_CAPACITY_STEP must be 1 or greater");

    if (HASHMAP_LOAD_FACTOR < 0 || 1 < HASHMAP_LOAD_FACTOR)
        HM_THROW(NULL, "HASHMAP_LOAD_FACTOR must be between 0 and 1");

    hm->size = 0;
    hm->capacity = capacity;
    hm->items = calloc(hm->capacity, sizeof(KV *));
    if (!hm->items) {
        free(hm);
        HM_THROW(NULL, "calloc error");
    }

    __hashmap_first_hash = INT_MAX;
    __hashmap_last_hash = 0;

    return hm;
}

KV *hashmap_new_kv(char *k, void *v) {
    if (k == NULL) HM_THROW(NULL, "k can't be NULL");

    KV *kv = malloc(sizeof(KV));
    if (!kv) HM_THROW(NULL, "malloc error");

    kv->key = k;
    kv->value = v;

    return kv;
}

bool hashmap_free_struct(HashMap *hm) {
    if (!hm) HM_THROW(false, "hm evaluates to false");

    free(hm->items);
    free(hm);
    hm = NULL;

    return true;
}

bool hashmap_free_kv_struct(KV *kv) {
    if (!kv) HM_THROW(false, "kv evaluates to false");

    free(kv);
    kv = NULL;

    return true;
}

bool hashmap_free(HashMap *hm) {
    if (!hm) HM_THROW(false, "hm evaluates to false");

    if (hm->capacity) {
        for (size_t i = 0; i < hm->capacity; i++) {
            if (!hm->size) break;
            if (!hm->items[i]) continue;

            if (!hashmap_free_kv(hm->items[i]))
                HM_THROW(false, "error freeing kv");

            hm->size--;
        }
    }

    return hashmap_free_struct(hm);
}

bool hashmap_free_kv(KV *kv) {
    if (!kv) HM_THROW(false, "kv evaluates to false");

    if (kv->value) free(kv->value);
    free(kv->key);

    return hashmap_free_kv_struct(kv);
}

bool hashmap_free_stack(HashMap *hm) {
    if (!hm) HM_THROW(false, "hm evaluates to false");

    if (hm->capacity) {
        for (size_t i = 0; i < hm->capacity; i++) {
            if (!hm->size) break;
            if (!hm->items[i]) continue;

            free(hm->items[i]);

            hm->size--;
        }
    }

    return hashmap_free_struct(hm);
}

void *hashmap_get(HashMap *hm, char *k) {
    if (!hm) HM_THROW(NULL, "hm evaluates to false");
    if (!hm->size) HM_THROW(NULL, "hashmap is uninitialized");
    if (!hm->capacity) HM_THROW(NULL, "capacity can't be 0");
    if (!hm->items) HM_THROW(NULL, "items evaluates to false");
    if (k == NULL) HM_THROW(NULL, "k can't be NULL");

    hash_t hash = hashmap_hash(k, hm->capacity);
    if (!hm->items[hash]) return NULL;

    return hm->items[hash]->value;
}

bool hashmap_set(HashMap *hm, char *k, void *v) {
    // TODO handle case where redefining a key causes a memory leak
    if (!hm) HM_THROW(false, "hm evaluates to false");
    if (!hm->capacity) HM_THROW(false, "capacity can't be 0");
    if (!hm->items) HM_THROW(false, "items evaluates to false");
    if (k == NULL) HM_THROW(false, "k can't be NULL");

    if (hm->size / hm->capacity >= HASHMAP_LOAD_FACTOR)
        if (!hashmap_resize(hm, hm->capacity + HASHMAP_CAPACITY_STEP))
            HM_THROW(false, "resize error");

    hash_t hash = hashmap_hash(k, hm->capacity);
    hm->items[hash] = hashmap_new_kv(k, v);
    hm->size++;

    if (hash < __hashmap_first_hash) __hashmap_first_hash = hash;
    if (hash > __hashmap_last_hash) __hashmap_last_hash = hash;

    return true;
}

bool hashmap_resize(HashMap *hm, size_t capacity) {
    if (!capacity) HM_THROW(false, "capacity can't be 0");

    if (!hm) HM_THROW(false, "hm evaluates to false");
    if (!hm->capacity) HM_THROW(false, "capacity can't be 0");
    if (!hm->items) HM_THROW(false, "items evaluates to false");

    if (hm->size > capacity)
        HM_THROW(false, "new capacity can't be less than current size");

    HashMap *tmp = hashmap_new(capacity);
    if (!tmp) HM_THROW(false, "error creating new HashMap");

    if (hm->size) {
        for (size_t i = 0; i < hm->capacity; i++) {
            if (!hm->items[i]) continue;

            hash_t hash = hashmap_hash(hm->items[i]->key, tmp->capacity);
            tmp->items[hash] = hm->items[i];
            tmp->size++;

            if (hash < __hashmap_first_hash) __hashmap_first_hash = hash;
            if (hash > __hashmap_last_hash) __hashmap_last_hash = hash;
        }
    }

    free(hm->items);

    *hm = *tmp;

    free(tmp);

    return true;
}

char **hashmap_keys(HashMap *hm) {
    if (!hm) HM_THROW(NULL, "hm evaluates to false");
    if (!hm->size) HM_THROW(NULL, "hashmap is uninitialized");
    if (!hm->capacity) HM_THROW(NULL, "capacity can't be 0");
    if (!hm->items) HM_THROW(NULL, "items evaluates to false");

    char **keys = malloc(hm->size * sizeof(char *));
    if (!keys) HM_THROW(NULL, "malloc error");

    size_t i = 0;
    for (size_t j = __hashmap_first_hash; j <= __hashmap_last_hash; j++) {
        if (!hm->items[j]) continue;
        printf("hash = %zu\n", j);
        keys[i++] = hm->items[j]->key;
    }

    return keys;
}

void **hashmap_values(HashMap *hm) {
    if (!hm) HM_THROW(NULL, "hm evaluates to false");
    if (!hm->size) HM_THROW(NULL, "hashmap is uninitialized");
    if (!hm->capacity) HM_THROW(NULL, "capacity can't be 0");
    if (!hm->items) HM_THROW(NULL, "items evaluates to false");

    void **values = malloc(hm->size * sizeof(void *));
    if (!values) HM_THROW(NULL, "malloc error");

    size_t i = 0;
    for (size_t j = __hashmap_first_hash; j <= __hashmap_last_hash; j++) {
        if (!hm->items[j]) continue;
        values[i++] = hm->items[j]->value;
    }

    return values;
}

KV **hashmap_items(HashMap *hm) {
    if (!hm) HM_THROW(NULL, "hm evaluates to false");
    if (!hm->size) HM_THROW(NULL, "hashmap is uninitialized");
    if (!hm->capacity) HM_THROW(NULL, "capacity can't be 0");
    if (!hm->items) HM_THROW(NULL, "items evaluates to false");

    KV **items = malloc(hm->size * sizeof(KV *));
    if (!items) HM_THROW(NULL, "malloc error");

    size_t i = 0;
    for (size_t j = __hashmap_first_hash; j <= __hashmap_last_hash; j++) {
        if (!hm->items[j]) continue;
        items[i++] = hm->items[j];
    }

    return items;
}

bool hashmap_eq(HashMap *hm1, HashMap *hm2) {
    if (!hm1) HM_THROW(false, "hm1 evaluates to false");
    if (!hm2) HM_THROW(false, "hm2 evaluates to false");

    if (hm1->size != hm2->size) HM_THROW(false, "sizes are different");

    for (size_t i = __hashmap_first_hash; i <= __hashmap_last_hash; i++) {
        KV *kv1 = hm1->items[i];
        KV *kv2 = hm2->items[i];
        if (!kv1 && !kv2) continue;

        if (!hashmap_eq_kv(kv1, kv2)) return false;
    }

    return true;
}

bool hashmap_eq_kv(KV *kv1, KV *kv2) {
    if (!kv1) HM_THROW(false, "kv1 evaluates to false");
    if (!kv2) HM_THROW(false, "kv2 evaluates to false");

    char *k1 = kv1->key;
    char *k2 = kv2->key;
    while (*k1 && *k2) if (*k1++ != *k2++) return false;

    char *v1 = kv1->value;
    char *v2 = kv2->value;
    while (*v1 && *v2) if (*v1++ != *v2++) return false;

    return true;
}

bool hashmap_print(HashMap *hm) {
    if (!hm) HM_THROW(false, "hm evaluates to false");

    printf("{\n");

    for (size_t i = __hashmap_first_hash; i <= __hashmap_last_hash; i++) {
        if (!hm->items[i]) continue;

        KV *kv = hm->items[i];
        printf("  \"%s\": \"%s\"\n", kv->key, (char *)kv->value);
    }

    printf("}\n");

    return true;
}

bool hashmap_print_kv(KV *kv) {
    if (!kv) HM_THROW(false, "kv evaluates to false");

    printf("key: %s\n", kv->key);
    printf("value: %s\n", (char *)kv->value);

    return true;
}

bool hashmap_info(HashMap *hm) {
    if (!hm) HM_THROW(false, "hm evaluates to false");

    printf("size: %zu\n", hm->size);
    printf("capacity: %zu\n", hm->capacity);

    return true;
}

#endif // HASHMAP_IMPLEMENTATION
