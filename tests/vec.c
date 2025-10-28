#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../vec.h"

bool vec_new_ok_string() {
    TEST("vec_new_ok_string");

    Vec *result = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_BOOL(result != NULL),
        ASSERT_BOOL(result->size == 0),
        ASSERT_BOOL(result->capacity == VEC_CAPACITY_STEP),
        ASSERT_BOOL(result->offset == sizeof(char *)),
        ASSERT_BOOL(result->items != NULL),
    );
}

bool vec_new_ok_int() {
    TEST("vec_new_ok_int");

    Vec *result = vec_new(sizeof(int));

    ASSERT_BLOCK(
        ASSERT_BOOL(result != NULL),
        ASSERT_BOOL(result->size == 0),
        ASSERT_BOOL(result->capacity == VEC_CAPACITY_STEP),
        ASSERT_BOOL(result->offset == sizeof(int)),
        ASSERT_BOOL(result->items != NULL),
    );
}

bool vec_new_err_capacity_step_lt_1() {
    TEST("vec_new_err_capacity_step_lt_1");

    #define VEC_CAPACITY_STEP 0
    Vec *result = vec_new(sizeof(int));
    #define VEC_CAPACITY_STEP 4

    ASSERT_BOOL(result == NULL);
}

bool vec_free_ok() {
    TEST("vec_free_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);
    if (!vec_push(v, "b")) ASSERT_BOOL(false);
    if (!vec_push(v, "c")) ASSERT_BOOL(false);

    bool result = vec_free(v);

    ASSERT_BOOL(result == true);
}

bool vec_free_err_v_is_null() {
    TEST("vec_free_err_v_is_null");

    ASSERT_BOOL(vec_free(NULL) == false);
}

bool vec_free_deep_ok() {
    TEST("vec_free_deep_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);
    if (!vec_push(v, "b")) ASSERT_BOOL(false);
    if (!vec_push(v, "c")) ASSERT_BOOL(false);

    bool result = vec_free_deep(v);

    ASSERT_BOOL(result == true);
}

bool vec_free_deep_err_v_is_null() {
    TEST("vec_free_deep_err_v_is_null");

    ASSERT_BOOL(vec_free_deep(NULL) == false);
}

bool vec_push_ok() {
    TEST("vec_push_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);
    if (!vec_push(v, "b")) ASSERT_BOOL(false);
    if (!vec_push(v, "c")) ASSERT_BOOL(false);

    ASSERT_BLOCK(
        ASSERT_BOOL(v != NULL),
        ASSERT_BOOL(v->size == 3),
        ASSERT_BOOL(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_BOOL(v->offset == sizeof(char *)),
        ASSERT_BOOL(v->items != NULL),
    );
}

bool vec_push_err_v_is_null() {
    TEST("vec_push_err_v_is_null");

    ASSERT_BOOL(vec_push(NULL, "a") == false);
}

bool vec_push_err_capacity_step_lt_1() {
    TEST("vec_push_err_capacity_step_lt_1");

    Vec *v = vec_new(sizeof(char *));

    #define VEC_CAPACITY_STEP 0
    bool result = vec_push(v, "a");
    #define VEC_CAPACITY_STEP 4

    ASSERT_BOOL(v->size == false);
}

bool vec_extend_ok() {
    TEST("vec_extend_ok");

    Vec *v1 = vec_new(sizeof(char *));
    vec_push(v1, "a");
    vec_push(v1, "b");

    Vec *v2 = vec_new(sizeof(char *));
    vec_push(v1, "c");
    vec_push(v1, "d");

    bool result = vec_extend(v1, v2);

    ASSERT_BLOCK(
        ASSERT_BOOL(result == true),
        ASSERT_STR(vec_get_as(v1, 0, char *), "a"),
        ASSERT_STR(vec_get_as(v1, 1, char *), "b"),
        ASSERT_STR(vec_get_as(v1, 2, char *), "c"),
        ASSERT_STR(vec_get_as(v1, 3, char *), "d"),
    );
}

bool vec_extend_err_dst_is_null() {
    TEST("vec_extend_err_dst_is_null");

    Vec *v1 = NULL;
    Vec *v2 = vec_new(sizeof(char *));

    bool result = vec_extend(v1, v2);

    ASSERT_BOOL(result == false);
}

bool vec_extend_err_src_size_is_0() {
    TEST("vec_extend_err_src_size_is_0");

    Vec *v1 = vec_new(sizeof(char *));
    Vec *v2 = vec_new(sizeof(char *));

    bool result = vec_extend(v1, v2);

    ASSERT_BOOL(result == false);
}

bool vec_extend_err_different_offsets() {
    TEST("vec_extend_err_different_offsets");

    Vec *v1 = vec_new(sizeof(char *));
    Vec *v2 = vec_new(sizeof(int));

    bool result = vec_extend(v1, v2);

    ASSERT_BOOL(result == false);
}

bool vec_get_ok() {
    TEST("vec_get_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);
    if (!vec_push(v, "b")) ASSERT_BOOL(false);
    if (!vec_push(v, "c")) ASSERT_BOOL(false);

    ASSERT_BLOCK(
        ASSERT_STR(vec_get(v, 0), "a"),
        ASSERT_STR(vec_get(v, 1), "b"),
        ASSERT_STR(vec_get(v, 2), "c"),
    );
}

bool vec_get_err_v_is_null() {
    TEST("vec_get_err_v_is_null");

    ASSERT_BOOL(vec_get(NULL, 0), NULL);
}

bool vec_get_err_v_size_is_0() {
    TEST("vec_get_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BOOL(vec_get(v, 1), NULL);
}

bool vec_get_err_index_gte_v_size() {
    TEST("vec_get_err_index_gte_v_size");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    ASSERT_BLOCK(
        ASSERT_BOOL(vec_get(v, 99), NULL),
    );
}

bool vec_get_ptr_ok() {
    TEST("vec_get_ptr_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    void *result = vec_get_ptr(v, 0);
    void *expected = "a";

    ASSERT_STR(result, expected);
}

bool vec_get_ptr_err_v_is_null() {
    TEST("vec_get_ptr_err_v_is_null");

    ASSERT_BOOL(vec_get_ptr(NULL, 0) == NULL);
}

bool vec_get_ptr_err_v_size_is_0() {
    TEST("vec_get_ptr_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BOOL(vec_get_ptr(v, 0) == NULL);
}

bool vec_get_ptr_err_index_gte_v_size() {
    TEST("vec_get_ptr_err_index_gte_v_size");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    ASSERT_BOOL(vec_get_ptr(v, 99) == NULL);
}

bool vec_get_as_ok() {
    TEST("vec_get_as_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    char *result = vec_get_as(v, 0, char *);
    char *expected = "a";

    ASSERT_STR(result, expected);
}

bool vec_get_as_err_v_is_null() {
    TEST("vec_get_as_err_v_is_null");

    ASSERT_BOOL(vec_get_as(NULL, 0, char *) == NULL);
}

bool vec_get_as_err_v_size_is_0() {
    TEST("vec_get_as_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BOOL(vec_get_as(v, 0, char *) == NULL);
}

bool vec_get_as_err_index_gte_v_size() {
    TEST("vec_get_as_err_index_gte_v_size");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    ASSERT_BOOL(vec_get_as(v, 99, char *) == NULL);
}

bool vec_print_ok() {
    TEST("vec_print_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    ASSERT_BOOL(vec_print(v) == true);
}

bool vec_print_err_v_is_null() {
    TEST("vec_print_err_v_is_null");

    ASSERT_BOOL(vec_print(NULL) == false);
}

bool vec_print_err_v_size_is_zero() {
    TEST("vec_print_err_v_size_is_zero");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BOOL(vec_print(v) == false);
}

bool vec_info_ok() {
    TEST("vec_info_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);

    ASSERT_BOOL(vec_info(v) == true);
}

bool vec_info_err_v_is_null() {
    TEST("vec_info_err_v_is_null");

    ASSERT_BOOL(vec_info(NULL) == false);
}

int main() {
    TEST_SUITE(
        // vec_new
        vec_new_ok_string,
        vec_new_ok_int,
        vec_new_err_capacity_step_lt_1,

        // vec_free
        vec_free_ok,
        vec_free_err_v_is_null,

        // vec_free_deep
        vec_free_deep_ok,
        vec_free_deep_err_v_is_null,

        // vec_push
        vec_push_ok,
        vec_push_err_v_is_null,
        vec_push_err_capacity_step_lt_1,

        // vec_extend
        vec_extend_ok,
        vec_extend_err_dst_is_null,
        vec_extend_err_src_size_is_0,
        vec_extend_err_different_offsets,

        // vec_get
        vec_get_ok,
        vec_get_err_v_is_null,
        vec_get_err_v_size_is_0,
        vec_get_err_index_gte_v_size,

        // vec_get_ptr
        vec_get_ptr_ok,
        vec_get_ptr_err_v_is_null,
        vec_get_ptr_err_v_size_is_0,
        vec_get_ptr_err_index_gte_v_size,

        // vec_get_as
        vec_get_as_ok,
        vec_get_as_err_v_is_null,
        vec_get_as_err_v_size_is_0,
        vec_get_as_err_index_gte_v_size,

        // vec_print
        vec_print_ok,
        vec_print_err_v_is_null,
        vec_print_err_v_size_is_zero,

        // vec_info
        vec_info_ok,
        vec_info_err_v_is_null,
    );
}
