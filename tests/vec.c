#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../vec.h"

bool test_vec_new_ok_string() {
    TEST("test_vec_new_ok_string");

    Vec *result = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_BOOL(result != NULL),
        ASSERT_BOOL(result->size == 0),
        ASSERT_BOOL(result->capacity == VEC_CAPACITY_STEP),
        ASSERT_BOOL(result->offset == sizeof(char *)),
        ASSERT_BOOL(result->items != NULL),
    );
}

bool test_vec_new_ok_int() {
    TEST("test_vec_new_ok_int");

    Vec *result = vec_new(sizeof(int));

    ASSERT_BLOCK(
        ASSERT_BOOL(result != NULL),
        ASSERT_BOOL(result->size == 0),
        ASSERT_BOOL(result->capacity == VEC_CAPACITY_STEP),
        ASSERT_BOOL(result->offset == sizeof(int)),
        ASSERT_BOOL(result->items != NULL),
    );
}

bool test_vec_new_err_capacity_step_lt_1() {
    TEST("test_vec_new_err_capacity_step_lt_1");

    #define VEC_CAPACITY_STEP 0
    Vec *result = vec_new(sizeof(int));
    #define VEC_CAPACITY_STEP 4

    ASSERT_BOOL(result == NULL);
}

bool test_vec_push_ok() {
    TEST("test_vec_push_ok");

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

bool test_vec_push_err_capacity_step_lt_1() {
    TEST("test_vec_push_err_capacity_step_lt_1");

    Vec *v = vec_new(sizeof(char *));

    #define VEC_CAPACITY_STEP 0
    bool result = vec_push(v, "a");
    #define VEC_CAPACITY_STEP 4

    ASSERT_BOOL(v->size == false);
}

bool test_vec_get_ok() {
    TEST("test_vec_get_ok");

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

bool test_vec_get_err_v_size_is_0() {
    TEST("test_vec_get_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_BOOL(vec_get(v, 1), NULL),
    );
}

bool test_vec_get_err_index_gte_v_size() {
    TEST("test_vec_get_err_index_gte_v_size");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) ASSERT_BOOL(false);
    if (!vec_push(v, "b")) ASSERT_BOOL(false);

    ASSERT_BLOCK(
        ASSERT_BOOL(vec_get(v, 99), NULL),
    );
}

int main() {
    TEST_SUITE(
        // vec_new
        test_vec_new_ok_string,
        test_vec_new_ok_int,
        test_vec_new_err_capacity_step_lt_1,

        // vec_free

        // vec_free_deep

        // vec_push
        test_vec_push_ok,
        test_vec_push_err_capacity_step_lt_1,

        // vec_extend

        // vec_get
        test_vec_get_ok,
        test_vec_get_err_v_size_is_0,
        test_vec_get_err_index_gte_v_size,

        // vec_get_ptr

        // vec_print

        // vec_info
    );
}
