#include "_test.h"
#include "../includes/vec.h"

// TODO free memory on tests

bool vec_new_ok_string() {
    TEST("vec_new_ok_string");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_ZERO(v->size),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_TRUE(v->offset == sizeof(char *)),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_new_ok_int() {
    TEST("vec_new_ok_int");

    Vec *v = vec_new(sizeof(int));

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_ZERO(v->size),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_TRUE(v->offset == sizeof(int)),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_new_err_offset_step_lt_1() {
    TEST("vec_new_err_offset_step_lt_1");

    ASSERT_NULL(vec_new(0));
}

bool vec_free_ok() {
    TEST("vec_free_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();
    if (!vec_push(v, str_clone("b"))) FAIL();
    if (!vec_push(v, str_clone("c"))) FAIL();

    ASSERT_TRUE(vec_free(v));
}

bool vec_free_err_v_is_null() {
    TEST("vec_free_err_v_is_null");

    ASSERT_FALSE(vec_free(NULL));
}

bool vec_push_ok() {
    TEST("vec_push_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();
    if (!vec_push(v, str_clone("b"))) FAIL();
    if (!vec_push(v, str_clone("c"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_TRUE(v->offset == sizeof(char *)),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_push_err_v_is_null() {
    TEST("vec_push_err_v_is_null");

    ASSERT_FALSE(vec_push(NULL, "a"));
}

bool vec_extend_ok() {
    TEST("vec_extend_ok");

    Vec *v1 = vec_new(sizeof(char *));
    vec_push(v1, str_clone("a"));
    vec_push(v1, str_clone("b"));

    Vec *v2 = vec_new(sizeof(char *));
    vec_push(v2, str_clone("c"));
    vec_push(v2, str_clone("d"));

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_extend(v1, v2)),
        ASSERT_STR(vec_get_as(v1, 0, char *), "a"),
        ASSERT_STR(vec_get_as(v1, 1, char *), "b"),
        ASSERT_STR(vec_get_as(v1, 2, char *), "c"),
        ASSERT_STR(vec_get_as(v1, 3, char *), "d"),

        // TODO
        // ASSERT_TRUE(vec_free(v1)),
        // ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_extend_err_dst_is_null() {
    TEST("vec_extend_err_dst_is_null");

    Vec *v1 = NULL;
    Vec *v2 = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_extend(v1, v2)),
        ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_extend_err_src_size_is_0() {
    TEST("vec_extend_err_src_size_is_0");

    Vec *v1 = vec_new(sizeof(char *));
    Vec *v2 = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_extend(v1, v2)),

        ASSERT_TRUE(vec_free(v1)),
        ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_extend_err_different_offsets() {
    TEST("vec_extend_err_different_offsets");

    Vec *v1 = vec_new(sizeof(char *));
    Vec *v2 = vec_new(sizeof(int));

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_extend(v1, v2)),

        ASSERT_TRUE(vec_free(v1)),
        ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_get_ok_positive_index() {
    TEST("vec_get_ok_positive_index");

    Vec *v = vec_new(sizeof(char *));

    if (!vec_push(v, str_clone("a"))) FAIL();
    if (!vec_push(v, str_clone("b"))) FAIL();
    if (!vec_push(v, str_clone("c"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_STR(vec_get(v, 0), "a"),
        ASSERT_STR(vec_get(v, 1), "b"),
        ASSERT_STR(vec_get(v, 2), "c"),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_ok_negative_index() {
    TEST("vec_get_ok_negative_index");

    Vec *v = vec_new(sizeof(char *));

    if (!vec_push(v, str_clone("a"))) FAIL();
    if (!vec_push(v, str_clone("b"))) FAIL();
    if (!vec_push(v, str_clone("c"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_STR(vec_get(v, -3), "a"),
        ASSERT_STR(vec_get(v, -2), "b"),
        ASSERT_STR(vec_get(v, -1), "c"),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_err_v_is_null() {
    TEST("vec_get_err_v_is_null");

    ASSERT_NULL(vec_get(NULL, 0));
}

bool vec_get_err_v_size_is_0() {
    TEST("vec_get_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, 1)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_err_positive_index_oob() {
    TEST("vec_get_err_positive_index_oob");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, 99)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_err_negative_index_oob() {
    TEST("vec_get_err_negative_index_oob");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, -99)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_as_ok() {
    TEST("vec_get_as_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_STR(vec_get_as(v, 0, char *), "a"),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_as_err_v_is_null() {
    TEST("vec_get_as_err_v_is_null");

    ASSERT_NULL(vec_get_as(NULL, 0, char *));
}

bool vec_get_as_err_v_size_is_0() {
    TEST("vec_get_as_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get_as(v, 0, char *)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_as_err_index_gte_v_size() {
    TEST("vec_get_as_err_index_gte_v_size");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get_as(v, 99, char *)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_print_ok() {
    TEST("vec_print_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, "a")) FAIL();

    ASSERT_TRUE(vec_print(v));
}

bool vec_print_err_v_is_null() {
    TEST("vec_print_err_v_is_null");

    ASSERT_FALSE(vec_print(NULL));
}

bool vec_print_err_v_size_is_0() {
    TEST("vec_print_err_v_size_is_0");

    Vec *v = vec_new(sizeof(char *));

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_print(v)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_info_ok() {
    TEST("vec_info_ok");

    Vec *v = vec_new(sizeof(char *));
    if (!vec_push(v, str_clone("a"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_info(v)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_info_err_v_is_null() {
    TEST("vec_info_err_v_is_null");

    ASSERT_FALSE(vec_info(NULL));
}

int main() {
    TEST_SUITE(
        // vec_new
        vec_new_ok_string,
        vec_new_ok_int,
        vec_new_err_offset_step_lt_1,

        // vec_free
        vec_free_ok,
        vec_free_err_v_is_null,

        // vec_push
        vec_push_ok,
        vec_push_err_v_is_null,

        // vec_extend
        vec_extend_ok,
        vec_extend_err_dst_is_null,
        vec_extend_err_src_size_is_0,
        vec_extend_err_different_offsets,

        // vec_get
        vec_get_ok_positive_index,
        vec_get_ok_negative_index,
        vec_get_err_v_is_null,
        vec_get_err_v_size_is_0,
        vec_get_err_positive_index_oob,
        vec_get_err_negative_index_oob,

        // vec_get_as
        vec_get_as_ok,
        vec_get_as_err_v_is_null,
        vec_get_as_err_v_size_is_0,
        vec_get_as_err_index_gte_v_size,

        // vec_print
        vec_print_ok,
        vec_print_err_v_is_null,
        vec_print_err_v_size_is_0,

        // vec_info
        vec_info_ok,
        vec_info_err_v_is_null,
    );
}
