#include "_test.h"

#define HASHMAP_HASH_ALGORITHM djb2
#include "../hashmap.h"

// TODO free memory on tests

bool hashmap_new_ok() {
    TEST("hashmap_new_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(hm->items),
        ASSERT_ZERO(hm->size),
        ASSERT(hm->capacity == HASHMAP_CAPACITY_STEP),
    );
}

bool hashmap_new_err_capacity_lt_1() {
    TEST("hashmap_new_err_capacity_lt_1");

    ASSERT_NULL(hashmap_new(0));
}

bool hashmap_new_kv_ok() {
    TEST("hashmap_new_kv_ok");

    KV *kv = hashmap_new_kv("red", "cherry");

    ASSERT_BLOCK(
        ASSERT_STR(kv->key,     "red"),
        ASSERT_STR(kv->value,   "cherry"),
    );
}

bool hashmap_new_kv_ok_v_is_null() {
    TEST("hashmap_new_kv_ok_v_is_null");

    KV *kv = hashmap_new_kv("red", NULL);

    ASSERT_BLOCK(
        ASSERT_STR(kv->key, "red"),
        ASSERT_NULL(kv->value),
    );}

bool hashmap_new_kv_err_k_is_null() {
    TEST("hashmap_new_kv_err_k_is_null");

    ASSERT_NULL(hashmap_new_kv(NULL, "cherry"));
}

bool hashmap_free_ok() {
    TEST("hashmap_free_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    hashmap_set(hm, "red",      "cherry");
    hashmap_set(hm, "orange",   "tangerine");
    hashmap_set(hm, "yellow",   "banana");
    hashmap_set(hm, "green",    "apple");
    hashmap_set(hm, "blue",     "blueberry");
    hashmap_set(hm, "purple",   "grape");

    ASSERT_TRUE(hashmap_free(hm));
}

bool hashmap_free_err_hm_is_null() {
    TEST("hashmap_free_err_hm_is_null");

    ASSERT_FALSE(hashmap_free(NULL));
}

bool hashmap_free_kv_ok() {
    TEST("hashmap_free_kv_ok");
    
    KV *kv = hashmap_new_kv("red", "cherry");

    ASSERT_TRUE(hashmap_free_kv(kv));
}

bool hashmap_free_kv_ok_v_is_null() {
    TEST("hashmap_free_kv_ok_v_is_null");

    KV *kv = hashmap_new_kv("red", NULL);

    ASSERT_TRUE(hashmap_free_kv(kv));
}

bool hashmap_free_kv_err_kv_is_null() {
    TEST("hashmap_free_kv_err_kv_is_null");

    ASSERT_FALSE(hashmap_free_kv(NULL));
}

bool hashmap_get_ok() {
    TEST("hashmap_get_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    hashmap_set(hm, "red",      "cherry");
    hashmap_set(hm, "orange",   "tangerine");
    hashmap_set(hm, "yellow",   "banana");
    hashmap_set(hm, "green",    "apple");
    hashmap_set(hm, "blue",     "blueberry");
    hashmap_set(hm, "purple",   "grape");

    ASSERT_BLOCK(
        ASSERT_STR(hashmap_get(hm, "red"   ), "cherry"),
        ASSERT_STR(hashmap_get(hm, "orange"), "tangerine"),
        ASSERT_STR(hashmap_get(hm, "yellow"), "banana"),
        ASSERT_STR(hashmap_get(hm, "green" ), "apple"),
        ASSERT_STR(hashmap_get(hm, "blue"  ), "blueberry"),
        ASSERT_STR(hashmap_get(hm, "purple"), "grape"),
    );
}

bool hashmap_get_err_hm_is_null() {
    TEST("hashmap_get_err_hm_is_null");

    ASSERT_NULL(hashmap_get(NULL, "red"));
}

bool hashmap_get_err_hm_size_is_0() {
    TEST("hashmap_get_err_hm_size_is_0");
    
    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_NULL(hashmap_get(hm, "red"));
}

bool hashmap_get_err_hm_items_is_null() {
    TEST("hashmap_get_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->size = 1;

    ASSERT_NULL(hashmap_get(hm, "red"));
}

bool hashmap_get_err_k_is_null() {
    TEST("hashmap_get_err_k_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hashmap_set(hm, "red", "cherry");

    ASSERT_NULL(hashmap_get(hm, NULL));
}

bool hashmap_set_ok() {
    TEST("hashmap_set_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_set(hm, "red",    "cherry")),
        ASSERT_TRUE(hashmap_set(hm, "orange", "tangerine")),
        ASSERT_TRUE(hashmap_set(hm, "yellow", "banana")),
        ASSERT_TRUE(hashmap_set(hm, "green",  "apple")),
        ASSERT_TRUE(hashmap_set(hm, "blue",   "blueberry")),
        ASSERT_TRUE(hashmap_set(hm, "purple", "grape")),

        ASSERT(hm->size == 6),
        ASSERT(hm->capacity == HASHMAP_CAPACITY_STEP),
    );
}

bool hashmap_set_ok_after_resize() {
    TEST("hashmap_set_ok_after_resize");

    HashMap *hm = hashmap_new(1);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_set(hm, "red",    "cherry")),
        ASSERT_TRUE(hashmap_set(hm, "orange", "tangerine")),
        ASSERT_TRUE(hashmap_set(hm, "yellow", "banana")),
        ASSERT_TRUE(hashmap_set(hm, "green",  "apple")),
        ASSERT_TRUE(hashmap_set(hm, "blue",   "blueberry")),
        ASSERT_TRUE(hashmap_set(hm, "purple", "grape")),

        ASSERT(hm->size == 6),
        ASSERT(hm->capacity == 1 + HASHMAP_CAPACITY_STEP),
    );
}

bool hashmap_set_err_hm_is_null() {
    TEST("hashmap_set_err_hm_is_null");

    ASSERT_FALSE(hashmap_set(NULL, "red", "cherry"));
}

bool hashmap_set_err_hm_capacity_is_0() {
    TEST("hashmap_set_err_hm_capacity_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->capacity = 0;

    ASSERT_FALSE(hashmap_set(hm, "red", "cherry"));
}

bool hashmap_set_err_hm_items_is_null() {
    TEST("hashmap_set_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->items = NULL;

    ASSERT_FALSE(hashmap_set(hm, "red", "cherry"));
}

bool hashmap_set_err_k_is_null() {
    TEST("hashmap_set_err_k_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_FALSE(hashmap_set(hm, NULL, "cherry"));
}

bool hashmap_resize_ok() {
    TEST("hashmap_resize_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    hashmap_set(hm, "red",    "cherry");
    hashmap_set(hm, "orange", "tangerine");
    hashmap_set(hm, "yellow", "banana");
    hashmap_set(hm, "green",  "apple");
    hashmap_set(hm, "blue",   "blueberry");
    hashmap_set(hm, "purple", "grape");

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_resize(hm, hm->capacity * 2)),

        ASSERT(hm->size == 6),
        ASSERT(hm->capacity == HASHMAP_CAPACITY_STEP * 2),

        ASSERT_STR(hashmap_get(hm, "red"),    "cherry"),
        ASSERT_STR(hashmap_get(hm, "orange"), "tangerine"),
        ASSERT_STR(hashmap_get(hm, "yellow"), "banana"),
        ASSERT_STR(hashmap_get(hm, "green"),  "apple"),
        ASSERT_STR(hashmap_get(hm, "blue"),   "blueberry"),
        ASSERT_STR(hashmap_get(hm, "purple"), "grape"),
    );
}

bool hashmap_resize_ok_hm_size_is_0() {
    TEST("hashmap_resize_ok_hm_size_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_resize(hm, hm->capacity * 2)),

        ASSERT_ZERO(hm->size),
        ASSERT(hm->capacity == HASHMAP_CAPACITY_STEP * 2),

        ASSERT_STR(hashmap_get(hm, "red"),    "cherry"),
        ASSERT_STR(hashmap_get(hm, "orange"), "tangerine"),
        ASSERT_STR(hashmap_get(hm, "yellow"), "banana"),
        ASSERT_STR(hashmap_get(hm, "green"),  "apple"),
        ASSERT_STR(hashmap_get(hm, "blue"),   "blueberry"),
        ASSERT_STR(hashmap_get(hm, "purple"), "grape"),
    );
}

bool hashmap_resize_err_capacity_is_0() {
    TEST("hashmap_resize_err_capacity_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_FALSE(hashmap_resize(hm, 0));
}

bool hashmap_resize_err_hm_is_null() {
    TEST("hashmap_resize_err_hm_is_null");

    ASSERT_FALSE(hashmap_resize(NULL, HASHMAP_CAPACITY_STEP));
}

bool hashmap_resize_err_hm_capacity_is_0() {
    TEST("hashmap_resize_err_hm_capacity_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_FALSE(hashmap_resize(hm, HASHMAP_CAPACITY_STEP));
}

bool hashmap_resize_err_hm_items_is_null() {
    TEST("hashmap_resize_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->items = NULL;

    ASSERT_FALSE(hashmap_resize(hm, HASHMAP_CAPACITY_STEP));
}

bool hashmap_resize_err_hm_size_gt_new_capacity() {
    TEST("hashmap_resize_err_hm_size_gt_new_capacity");

    ASSERT(false);
}

int main() {
    TEST_SUITE(
        // hashmap_new
        hashmap_new_ok,
        hashmap_new_err_capacity_lt_1,

        // hashmap_new_kv
        hashmap_new_kv_ok,
        hashmap_new_kv_ok_v_is_null,
        hashmap_new_kv_err_k_is_null,

        // hashmap_free
        hashmap_free_ok,
        hashmap_free_err_hm_is_null,

        // hashmap_free_kv
        hashmap_free_kv_ok,
        hashmap_free_kv_ok_v_is_null,
        hashmap_free_kv_err_kv_is_null,

        // hashmap_get
        hashmap_get_ok,
        hashmap_get_err_hm_is_null,
        hashmap_get_err_hm_size_is_0,
        hashmap_get_err_hm_items_is_null,
        hashmap_get_err_k_is_null,

        // hashmap_set
        hashmap_set_ok,
        hashmap_set_ok_after_resize,
        hashmap_set_err_hm_is_null,
        hashmap_set_err_hm_capacity_is_0,
        hashmap_set_err_hm_items_is_null,
        hashmap_set_err_k_is_null,

        // hashmap_resize
        hashmap_resize_ok,
        hashmap_resize_ok_hm_size_is_0,
        hashmap_resize_err_capacity_is_0,
        hashmap_resize_err_hm_is_null,
        hashmap_resize_err_hm_capacity_is_0,
        hashmap_resize_err_hm_items_is_null,
        hashmap_resize_err_hm_size_gt_new_capacity,
    );
}
