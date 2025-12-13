#include "_test.h"

#define HASHMAP_HASH_ALGORITHM djb2
#include "../includes/hashmap.h"

// TODO tests for hashmap_keys
// TODO tests for hashmap_values
// TODO tests for hashmap_items

HashMap *_fixture_hashmap_heap(size_t cap) {
    HashMap *hm = hashmap_new(cap);

    hashmap_set(hm, str_clone("red"),    str_clone("cherry"));
    hashmap_set(hm, str_clone("orange"), str_clone("tangerine"));
    hashmap_set(hm, str_clone("yellow"), str_clone("banana"));
    hashmap_set(hm, str_clone("green"),  str_clone("apple"));
    hashmap_set(hm, str_clone("blue"),   str_clone("blueberry"));
    hashmap_set(hm, str_clone("purple"), str_clone("grape"));

    return hm;
}

HashMap *_fixture_hashmap_stack(size_t cap) {
    HashMap *hm = hashmap_new(cap);

    hashmap_set(hm, "red",    "cherry");
    hashmap_set(hm, "orange", "tangerine");
    hashmap_set(hm, "yellow", "banana");
    hashmap_set(hm, "green",  "apple");
    hashmap_set(hm, "blue",   "blueberry");
    hashmap_set(hm, "purple", "grape");

    return hm;
}

bool hashmap_new_ok() {
    TEST("hashmap_new_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(hm->items),
        ASSERT_ZERO(hm->size),
        ASSERT_TRUE(hm->capacity == HASHMAP_CAPACITY_STEP),

        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_new_err_capacity_lt_1() {
    TEST("hashmap_new_err_capacity_lt_1");

    ASSERT_NULL(hashmap_new(0));
}

bool hashmap_new_kv_ok() {
    TEST("hashmap_new_kv_ok");

    KV *kv = hashmap_new_kv(str_clone("red"), str_clone("cherry"));

    ASSERT_BLOCK(
        ASSERT_STR(kv->key,     "red"),
        ASSERT_STR(kv->value,   "cherry"),

        ASSERT_TRUE(hashmap_free_kv(kv)),
    );
}

bool hashmap_new_kv_ok_v_is_null() {
    TEST("hashmap_new_kv_ok_v_is_null");

    KV *kv = hashmap_new_kv(str_clone("red"), NULL);

    ASSERT_BLOCK(
        ASSERT_STR(kv->key, "red"),
        ASSERT_NULL(kv->value),

        ASSERT_TRUE(hashmap_free_kv(kv)),
    );
}

bool hashmap_new_kv_err_k_is_null() {
    TEST("hashmap_new_kv_err_k_is_null");

    ASSERT_NULL(hashmap_new_kv(NULL, "cherry"));
}

bool hashmap_free_struct_ok() {
    TEST("hashmap_free_struct_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_TRUE(hashmap_free_struct(hm));
}

bool hashmap_free_struct_err_hm_is_null() {
    TEST("hashmap_free_struct_err_hm_is_null");

    ASSERT_FALSE(hashmap_free_struct(NULL));
}

bool hashmap_free_kv_struct_ok() {
    TEST("hashmap_free_kv_struct_ok");

    KV *kv = hashmap_new_kv("red", "cherry");

    ASSERT_TRUE(hashmap_free_kv_struct(kv));
}

bool hashmap_free_kv_struct_err_hm_is_null() {
    TEST("hashmap_free_kv_struct_err_hm_is_null");

    ASSERT_FALSE(hashmap_free_kv_struct(NULL));
}

bool hashmap_free_ok() {
    TEST("hashmap_free_ok");

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    ASSERT_TRUE(hashmap_free(hm));
}

bool hashmap_free_ok_value_is_null() {
    TEST("hashmap_free_ok_value_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    hashmap_set(hm, str_clone("red"),    NULL);
    hashmap_set(hm, str_clone("orange"), NULL);
    hashmap_set(hm, str_clone("yellow"), NULL);
    hashmap_set(hm, str_clone("green"),  NULL);
    hashmap_set(hm, str_clone("blue"),   NULL);
    hashmap_set(hm, str_clone("purple"), NULL);

    ASSERT_TRUE(hashmap_free(hm));
}

bool hashmap_free_err_hm_is_null() {
    TEST("hashmap_free_err_hm_is_null");

    ASSERT_FALSE(hashmap_free(NULL));
}

bool hashmap_free_kv_ok() {
    TEST("hashmap_free_kv_ok");

    KV *kv = hashmap_new_kv(str_clone("red"), str_clone("cherry"));

    ASSERT_TRUE(hashmap_free_kv(kv));
}

bool hashmap_free_kv_ok_v_is_null() {
    TEST("hashmap_free_kv_ok_v_is_null");

    KV *kv = hashmap_new_kv(str_clone("red"), NULL);

    ASSERT_TRUE(hashmap_free_kv(kv));
}

bool hashmap_free_kv_err_kv_is_null() {
    TEST("hashmap_free_kv_err_kv_is_null");

    ASSERT_FALSE(hashmap_free_kv(NULL));
}

bool hashmap_free_stack_ok() {
    TEST("hashmap_free_stack_ok");

    HashMap *hm = _fixture_hashmap_stack(HASHMAP_CAPACITY_STEP);

    ASSERT_TRUE(hashmap_free_stack(hm));
}

bool hashmap_free_stack_err_hm_is_null() {
    TEST("hashmap_free_stack_err_hm_is_null");

    ASSERT_FALSE(hashmap_free_stack(NULL));
}

bool hashmap_get_ok() {
    TEST("hashmap_get_ok");

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_STR(hashmap_get(hm, "red"),    "cherry"),
        ASSERT_STR(hashmap_get(hm, "orange"), "tangerine"),
        ASSERT_STR(hashmap_get(hm, "yellow"), "banana"),
        ASSERT_STR(hashmap_get(hm, "green"),  "apple"),
        ASSERT_STR(hashmap_get(hm, "blue"),   "blueberry"),
        ASSERT_STR(hashmap_get(hm, "purple"), "grape"),

        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_get_err_hm_is_null() {
    TEST("hashmap_get_err_hm_is_null");

    ASSERT_NULL(hashmap_get(NULL, "red"));
}

bool hashmap_get_err_hm_size_is_0() {
    TEST("hashmap_get_err_hm_size_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_NULL(hashmap_get(hm, "red")),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_get_err_hm_items_is_null() {
    TEST("hashmap_get_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    free(hm->items);
    hm->items = NULL;

    ASSERT_BLOCK(
        ASSERT_NULL(hashmap_get(hm, "red")),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_get_err_k_is_null() {
    TEST("hashmap_get_err_k_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hashmap_set(hm, str_clone("red"), str_clone("cherry"));

    ASSERT_BLOCK(
        ASSERT_NULL(hashmap_get(hm, NULL)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_set_ok() {
    TEST("hashmap_set_ok");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_set(hm, str_clone("red"),    str_clone("cherry"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("orange"), str_clone("tangerine"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("yellow"), str_clone("banana"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("green"),  str_clone("apple"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("blue"),   str_clone("blueberry"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("purple"), str_clone("grape"))),

        ASSERT_TRUE(hm->size == 6),
        ASSERT_TRUE(hm->capacity == HASHMAP_CAPACITY_STEP),

        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_set_ok_after_resize() {
    TEST("hashmap_set_ok_after_resize");

    HashMap *hm = hashmap_new(1);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_set(hm, str_clone("red"),    str_clone("cherry"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("orange"), str_clone("tangerine"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("yellow"), str_clone("banana"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("green"),  str_clone("apple"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("blue"),   str_clone("blueberry"))),
        ASSERT_TRUE(hashmap_set(hm, str_clone("purple"), str_clone("grape"))),

        ASSERT_TRUE(hm->size == 6),
        ASSERT_TRUE(hm->capacity == 1 + HASHMAP_CAPACITY_STEP),

        ASSERT_TRUE(hashmap_free(hm)),
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

    char *key = str_clone("red");
    char *value = str_clone("cherry");

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_set(hm, key, value)),

        ASSERT_TRUE(hashmap_free(hm)),
        FREE(key),
        FREE(value),
    );
}

bool hashmap_set_err_hm_items_is_null() {
    TEST("hashmap_set_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    free(hm->items);
    hm->items = NULL;

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_set(hm, "red", "cherry")),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_set_err_k_is_null() {
    TEST("hashmap_set_err_k_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_set(hm, NULL, "cherry")),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_ok() {
    TEST("hashmap_resize_ok");

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_resize(hm, hm->capacity * 2)),

        ASSERT_TRUE(hm->size == 6),
        ASSERT_TRUE(hm->capacity == HASHMAP_CAPACITY_STEP * 2),

        ASSERT_STR(hashmap_get(hm, "red"),    "cherry"),
        ASSERT_STR(hashmap_get(hm, "orange"), "tangerine"),
        ASSERT_STR(hashmap_get(hm, "yellow"), "banana"),
        ASSERT_STR(hashmap_get(hm, "green"),  "apple"),
        ASSERT_STR(hashmap_get(hm, "blue"),   "blueberry"),
        ASSERT_STR(hashmap_get(hm, "purple"), "grape"),

        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_ok_hm_size_is_0() {
    TEST("hashmap_resize_ok_hm_size_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_resize(hm, hm->capacity * 2)),

        ASSERT_ZERO(hm->size),
        ASSERT_TRUE(hm->capacity == HASHMAP_CAPACITY_STEP * 2),

        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_err_capacity_is_0() {
    TEST("hashmap_resize_err_capacity_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_resize(hm, 0)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_err_hm_is_null() {
    TEST("hashmap_resize_err_hm_is_null");

    ASSERT_FALSE(hashmap_resize(NULL, HASHMAP_CAPACITY_STEP));
}

bool hashmap_resize_err_hm_capacity_is_0() {
    TEST("hashmap_resize_err_hm_capacity_is_0");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->capacity = 0;

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_resize(hm, HASHMAP_CAPACITY_STEP)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_err_hm_items_is_null() {
    TEST("hashmap_resize_err_hm_items_is_null");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);

    free(hm->items);
    hm->items = NULL;

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_resize(hm, HASHMAP_CAPACITY_STEP)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_resize_err_hm_size_gt_new_capacity() {
    TEST("hashmap_resize_err_hm_size_gt_new_capacity");

    HashMap *hm = hashmap_new(HASHMAP_CAPACITY_STEP);
    hm->size = 99;

    ASSERT_BLOCK(
        ASSERT_FALSE(hashmap_resize(hm, 0)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_keys_ok() {
    TEST("hashmap_keys_ok");
    SKIP();

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    char **result = hashmap_keys(hm);
    char *expected[] = {"red", "orange", "yellow", "green", "blue", "purple"};

    ASSERT_BLOCK(
        ASSERT_ARR_STR(
            result,   hm->size,
            expected, hm->size,
        ),
    );
}

bool hashmap_values_ok() {
    TEST("hashmap_values_ok");
    SKIP();

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    void **result = hashmap_values(hm);
    char *expected[] = {"cherry", "tangerine", "banana", "apple", "blueberry", "grape"};

    Vec *v1 = vec_from_arr(result, hm->size);
    Vec *v2 = vec_from(expected);

    if (!vec_sort(v1)) FAIL();
    if (!vec_sort(v2)) FAIL();

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_eq(v1, v2)),

        ASSERT_TRUE(vec_free(v1)),
        ASSERT_TRUE(vec_free(v2)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_items_ok() {
    TEST("hashmap_items_ok");
    SKIP();

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    KV **result = hashmap_items(hm);
    KV *expected[] = {
        hashmap_new_kv(str_clone("red"),    str_clone("cherry")),
        hashmap_new_kv(str_clone("orange"), str_clone("tangerine")),
        hashmap_new_kv(str_clone("yellow"), str_clone("banana")),
        hashmap_new_kv(str_clone("green"),  str_clone("apple")),
        hashmap_new_kv(str_clone("blue"),   str_clone("blueberry")),
        hashmap_new_kv(str_clone("purple"), str_clone("grape")),
    };

    for (size_t i = 0; i < hm->size; i++) {
        if (!hashmap_eq_kv(result[i], expected[i])) FAIL();
    }

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_free(hm)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_eq_ok() {
    TEST("hashmap_eq_ok");

    HashMap *hm1 = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);
    HashMap *hm2 = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_eq(hm1, hm2)),

        ASSERT_TRUE(hashmap_free(hm1)),
        ASSERT_TRUE(hashmap_free(hm2)),
    );
}

bool hashmap_eq_kv_ok() {
    TEST("hashmap_eq_kv_ok");

    KV *kv1 = hashmap_new_kv(str_clone("red"), str_clone("cherry"));
    KV *kv2 = hashmap_new_kv(str_clone("red"), str_clone("cherry"));

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_eq_kv(kv1, kv2)),

        ASSERT_TRUE(hashmap_free_kv(kv1)),
        ASSERT_TRUE(hashmap_free_kv(kv2)),
    );
}

bool hashmap_info_ok() {
    TEST("hashmap_info_ok");

    HashMap *hm = _fixture_hashmap_heap(HASHMAP_CAPACITY_STEP);

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_info(hm)),
        ASSERT_TRUE(hashmap_free(hm)),
    );
}

bool hashmap_info_err_hm_is_null() {
    TEST("hashmap_info_err_hm_is_null");

    ASSERT_FALSE(hashmap_info(NULL));
}

bool hashmap_info_kv_ok() {
    TEST("hashmap_info_kv_ok");

    KV *kv = hashmap_new_kv(str_clone("red"), str_clone("cherry"));

    ASSERT_BLOCK(
        ASSERT_TRUE(hashmap_info_kv(kv)),
        ASSERT_TRUE(hashmap_free_kv(kv)),
    );
}

bool hashmap_info_kv_err_kv_is_null() {
    TEST("hashmap_info_kv_err_kv_is_null");

    ASSERT_FALSE(hashmap_info_kv(NULL));
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

        // hashmap_free_struct
        hashmap_free_struct_ok,
        hashmap_free_struct_err_hm_is_null,

        // hashmap_free_kv_struct
        hashmap_free_kv_struct_ok,
        hashmap_free_kv_struct_err_hm_is_null,

        // hashmap_free
        hashmap_free_ok,
        hashmap_free_ok_value_is_null,
        hashmap_free_err_hm_is_null,

        // hashmap_free_kv
        hashmap_free_kv_ok,
        hashmap_free_kv_ok_v_is_null,
        hashmap_free_kv_err_kv_is_null,

        // hashmap_free_stack
        hashmap_free_stack_ok,
        hashmap_free_stack_err_hm_is_null,

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

        // hashmap_keys
        hashmap_keys_ok,

        // hashmap_values
        hashmap_values_ok,

        // hashmap_items
        hashmap_items_ok,

        // hashmap_eq
        hashmap_eq_ok,
        // TODO test error cases too

        // hashmap_eq_kv
        hashmap_eq_kv_ok,
        // TODO test error cases too

        // hashmap_info
        hashmap_info_ok,
        hashmap_info_err_hm_is_null,

        // hashmap_info_kv
        hashmap_info_kv_ok,
        hashmap_info_kv_err_kv_is_null,
    );
}
