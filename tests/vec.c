#include "_test.h"
#include "../includes/vec.h"

// TODO free memory on tests

bool vec_new_ok_string() {
    TEST("vec_new_ok_string");

    Vec *v = vec_new();

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_ZERO(v->size),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_new_ok_int() {
    TEST("vec_new_ok_int");

    Vec *v = vec_new();

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_ZERO(v->size),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_from_ok_stack_allocated() {
    TEST("vec_from_ok_stack_allocated");

    char *arr[] = { "foo", "bar", "baz" };

    Vec *v = vec_from(arr);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_STR(vec_get(v, 0), "foo"),
        ASSERT_STR(vec_get(v, 1), "bar"),
        ASSERT_STR(vec_get(v, 2), "baz"),

        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_from_ok_heap_allocated() {
    TEST("vec_from_ok_heap_allocated");

    char *arr[] = {
        str_clone("foo"),
        str_clone("bar"),
        str_clone("baz"),
    };

    Vec *v = vec_from(arr);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_STR(vec_get(v, 0), "foo"),
        ASSERT_STR(vec_get(v, 1), "bar"),
        ASSERT_STR(vec_get(v, 2), "baz"),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_from_err_arr_is_null() {
    TEST("vec_from_err_arr_is_null");

    ASSERT_NULL(vec_from(NULL));
}

bool vec_from_arr_ok_stack_allocated() {
    TEST("vec_from_arr_ok_stack_allocated");

    char *arr[] = { "foo", "bar", "baz" };

    Vec *v = vec_from_arr((void**)arr, 3);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_STR(vec_get(v, 0), "foo"),
        ASSERT_STR(vec_get(v, 1), "bar"),
        ASSERT_STR(vec_get(v, 2), "baz"),

        ASSERT_TRUE(vec_free_struct(v)),
    );
}
bool vec_from_arr_ok_heap_allocated() {
    TEST("vec_from_arr_ok_heap_allocated");

    char *arr[] = { str_clone("foo"), str_clone("bar"), str_clone("baz") };

    Vec *v = vec_from_arr((void**)arr, 3);

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_STR(vec_get(v, 0), "foo"),
        ASSERT_STR(vec_get(v, 1), "bar"),
        ASSERT_STR(vec_get(v, 2), "baz"),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_from_arr_err_arr_is_null() {
    TEST("vec_from_arr_err_arr_is_null");

    ASSERT_NULL(vec_from_arr(NULL, 99));
}

bool vec_free_struct_ok() {
    TEST("vec_free_struct_ok");

    Vec *v = vec_new();
    if (!vec_push(v, "a")) FAIL();
    if (!vec_push(v, "b")) FAIL();
    if (!vec_push(v, "c")) FAIL();

    ASSERT_TRUE(vec_free_struct(v));
}

bool vec_free_struct_err_v_is_null() {
    TEST("vec_free_struct_err_v_is_null");

    ASSERT_FALSE(vec_free_struct(NULL));
}

bool vec_free_ok() {
    TEST("vec_free_ok");

    Vec *v = vec_new();
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

    Vec *v = vec_new();
    if (!vec_push(v, str_clone("a"))) FAIL();
    if (!vec_push(v, str_clone("b"))) FAIL();
    if (!vec_push(v, str_clone("c"))) FAIL();

    ASSERT_BLOCK(
        ASSERT_NOT_NULL(v),
        ASSERT_TRUE(v->size == 3),
        ASSERT_TRUE(v->capacity == VEC_CAPACITY_STEP),
        ASSERT_NOT_NULL(v->items),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_push_err_v_is_null() {
    TEST("vec_push_err_v_is_null");

    ASSERT_FALSE(vec_push(NULL, "a"));
}

bool vec_extend_ok_stack_allocated() {
    TEST("vec_extend_ok_stack_allocated");

    Vec *v1 = vec_new();
    vec_push(v1, "a");
    vec_push(v1, "b");

    Vec *v2 = vec_new();
    vec_push(v2, "c");
    vec_push(v2, "d");

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_extend(v1, v2)),

        ASSERT_STR(vec_get(v1, 0), "a"),
        ASSERT_STR(vec_get(v1, 1), "b"),
        ASSERT_STR(vec_get(v1, 2), "c"),
        ASSERT_STR(vec_get(v1, 3), "d"),

        ASSERT_TRUE(vec_free_struct(v1)),
        ASSERT_TRUE(vec_free_struct(v2)),
    );
}

bool vec_extend_ok_heap_allocated() {
    TEST("vec_extend_ok_heap_allocated");

    Vec *v1 = vec_new();
    vec_push(v1, str_clone("a"));
    vec_push(v1, str_clone("b"));

    Vec *v2 = vec_new();
    vec_push(v2, str_clone("c"));
    vec_push(v2, str_clone("d"));

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_extend(v1, v2)),

        ASSERT_STR(vec_get(v1, 0), "a"),
        ASSERT_STR(vec_get(v1, 1), "b"),
        ASSERT_STR(vec_get(v1, 2), "c"),
        ASSERT_STR(vec_get(v1, 3), "d"),

        // TODO
        // ASSERT_TRUE(vec_free(v1)),
        // ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_extend_err_dst_is_null() {
    TEST("vec_extend_err_dst_is_null");

    Vec *v1 = NULL;
    Vec *v2 = vec_new();

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_extend(v1, v2)),
        ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_extend_err_src_size_is_0() {
    TEST("vec_extend_err_src_size_is_0");

    Vec *v1 = vec_new();
    Vec *v2 = vec_new();

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_extend(v1, v2)),

        ASSERT_TRUE(vec_free(v1)),
        ASSERT_TRUE(vec_free(v2)),
    );
}

bool vec_get_ok_positive_index() {
    TEST("vec_get_ok_positive_index");

    Vec *v = vec_new();

    if (!vec_push(v, "a")) FAIL();
    if (!vec_push(v, "b")) FAIL();
    if (!vec_push(v, "c")) FAIL();

    ASSERT_BLOCK(
        ASSERT_STR(vec_get(v, 0), "a"),
        ASSERT_STR(vec_get(v, 1), "b"),
        ASSERT_STR(vec_get(v, 2), "c"),

        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_get_ok_negative_index() {
    TEST("vec_get_ok_negative_index");

    Vec *v = vec_new();

    if (!vec_push(v, "a")) FAIL();
    if (!vec_push(v, "b")) FAIL();
    if (!vec_push(v, "c")) FAIL();

    ASSERT_BLOCK(
        ASSERT_STR(vec_get(v, -3), "a"),
        ASSERT_STR(vec_get(v, -2), "b"),
        ASSERT_STR(vec_get(v, -1), "c"),

        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_get_err_v_is_null() {
    TEST("vec_get_err_v_is_null");

    ASSERT_NULL(vec_get(NULL, 0));
}

bool vec_get_err_v_size_is_0() {
    TEST("vec_get_err_v_size_is_0");

    Vec *v = vec_new();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, 1)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_get_err_positive_index_oob() {
    TEST("vec_get_err_positive_index_oob");

    Vec *v = vec_new();
    if (!vec_push(v, "a")) FAIL();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, 99)),
        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_get_err_negative_index_oob() {
    TEST("vec_get_err_negative_index_oob");

    Vec *v = vec_new();
    if (!vec_push(v, "a")) FAIL();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_get(v, -99)),
        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_pop_ok_stack_allocated() {
    TEST("vec_pop_ok_stack_allocated");

    Vec *v = vec_new();
    vec_push(v, "a");
    vec_push(v, "b");
    vec_push(v, "c");

    ASSERT_BLOCK(
        ASSERT_STR(vec_pop(v), "c"),
        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_pop_ok_heap_allocated() {
    TEST("vec_pop_ok_heap_allocated");

    Vec *v = vec_new();
    vec_push(v, str_clone("a"));
    vec_push(v, str_clone("b"));
    vec_push(v, str_clone("c"));

    char *last = vec_pop(v);

    ASSERT_BLOCK(
        ASSERT_STR(last, "c"),
        ASSERT_TRUE(vec_free(v)),
        FREE(last),
    );
}

bool vec_pop_err_v_is_null() {
    TEST("vec_pop_err_v_is_null");

    ASSERT_NULL(vec_pop(NULL));
}

bool vec_pop_err_v_size_is_0() {
    TEST("vec_pop_err_v_size_is_0");

    Vec *v = vec_new();

    ASSERT_BLOCK(
        ASSERT_NULL(vec_pop(v)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_sort_ok_stack_allocated() {
    TEST("vec_sort_ok_stack_allocated");

    Vec *v = vec_new();
    vec_push(v, "b");
    vec_push(v, "d");
    vec_push(v, "e");
    vec_push(v, "c");
    vec_push(v, "a");

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_sort(v)),

        ASSERT_STR(vec_get(v, 0), "a"),
        ASSERT_STR(vec_get(v, 1), "b"),
        ASSERT_STR(vec_get(v, 2), "c"),
        ASSERT_STR(vec_get(v, 3), "d"),
        ASSERT_STR(vec_get(v, 4), "e"),

        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_sort_ok_heap_allocated() {
    TEST("vec_sort_ok_heap_allocated");

    Vec *v = vec_new();
    vec_push(v, str_clone("b"));
    vec_push(v, str_clone("d"));
    vec_push(v, str_clone("e"));
    vec_push(v, str_clone("c"));
    vec_push(v, str_clone("a"));

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_sort(v)),

        ASSERT_STR(vec_get(v, 0), "a"),
        ASSERT_STR(vec_get(v, 1), "b"),
        ASSERT_STR(vec_get(v, 2), "c"),
        ASSERT_STR(vec_get(v, 3), "d"),
        ASSERT_STR(vec_get(v, 4), "e"),

        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_sort_err_v_is_null() {
    TEST("vec_sort_err_v_is_null");

    ASSERT_FALSE(vec_sort(NULL));
}

bool vec_eq_ok_ordered_vectors() {
    TEST("vec_eq_ok_ordered_vectors");

    Vec *v1 = vec_new();
    vec_push(v1, "a");
    vec_push(v1, "b");
    vec_push(v1, "c");

    Vec *v2 = vec_new();
    vec_push(v2, "a");
    vec_push(v2, "b");
    vec_push(v2, "c");

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_eq(v1, v2)),

        ASSERT_TRUE(vec_free_struct(v1)),
        ASSERT_TRUE(vec_free_struct(v2)),
    );
}

bool vec_eq_ok_unordered_vectors() {
    TEST("vec_eq_ok_unordered_vectors");

    Vec *v1 = vec_new();
    vec_push(v1, "a");
    vec_push(v1, "b");
    vec_push(v1, "c");

    Vec *v2 = vec_new();
    vec_push(v2, "c");
    vec_push(v2, "b");
    vec_push(v2, "a");

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_eq(v1, v2)),

        ASSERT_TRUE(vec_free_struct(v1)),
        ASSERT_TRUE(vec_free_struct(v2)),
    );
}

bool vec_eq_err_v1_is_null() {
    TEST("vec_eq_err_v1_is_null");

    Vec *v1 = NULL;

    Vec *v2 = vec_new();
    vec_push(v2, "a");
    vec_push(v2, "b");
    vec_push(v2, "c");

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_eq(v1, v2)),
        ASSERT_TRUE(vec_free_struct(v2)),
    );
}

bool vec_eq_err_v2_is_null() {
    TEST("vec_eq_err_v2_is_null");

    Vec *v1 = vec_new();
    vec_push(v1, "a");
    vec_push(v1, "b");
    vec_push(v1, "c");

    Vec *v2 = NULL;

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_eq(v1, v2)),
        ASSERT_TRUE(vec_free_struct(v1)),
    );
}

bool vec_eq_err_different_sizes() {
    TEST("vec_eq_err_different_sizes");

    Vec *v1 = vec_new();
    v1->size = 0;

    Vec *v2 = vec_new();
    v2->size = 99;

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_eq(v1, v2)),

        ASSERT_TRUE(vec_free_struct(v1)),
        ASSERT_TRUE(vec_free_struct(v2)),
    );
}

bool vec_print_ok() {
    TEST("vec_print_ok");

    Vec *v = vec_new();
    if (!vec_push(v, "a")) FAIL();

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_print(v)),
        ASSERT_TRUE(vec_free_struct(v)),
    );
}

bool vec_print_err_v_is_null() {
    TEST("vec_print_err_v_is_null");

    ASSERT_FALSE(vec_print(NULL));
}

bool vec_print_err_v_size_is_0() {
    TEST("vec_print_err_v_size_is_0");

    Vec *v = vec_new();

    ASSERT_BLOCK(
        ASSERT_FALSE(vec_print(v)),
        ASSERT_TRUE(vec_free(v)),
    );
}

bool vec_info_ok() {
    TEST("vec_info_ok");

    Vec *v = vec_new();
    if (!vec_push(v, "a")) FAIL();

    ASSERT_BLOCK(
        ASSERT_TRUE(vec_info(v)),
        ASSERT_TRUE(vec_free_struct(v)),
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

        // vec_from
        vec_from_ok_stack_allocated,
        vec_from_ok_heap_allocated,
        vec_from_err_arr_is_null,

        // vec_from_arr
        vec_from_arr_ok_stack_allocated,
        vec_from_arr_ok_heap_allocated,
        vec_from_arr_err_arr_is_null,

        // vec_free_struct
        vec_free_struct_ok,
        vec_free_struct_err_v_is_null,

        // vec_free
        vec_free_ok,
        vec_free_err_v_is_null,

        // vec_push
        vec_push_ok,
        vec_push_err_v_is_null,

        // vec_extend
        vec_extend_ok_stack_allocated,
        vec_extend_ok_heap_allocated,
        vec_extend_err_dst_is_null,
        vec_extend_err_src_size_is_0,

        // vec_get
        vec_get_ok_positive_index,
        vec_get_ok_negative_index,
        vec_get_err_v_is_null,
        vec_get_err_v_size_is_0,
        vec_get_err_positive_index_oob,
        vec_get_err_negative_index_oob,

        // vec_pop
        vec_pop_ok_stack_allocated,
        vec_pop_ok_heap_allocated,
        vec_pop_err_v_is_null,
        vec_pop_err_v_size_is_0,

        // vec_sort
        vec_sort_ok_stack_allocated,
        vec_sort_ok_heap_allocated,
        vec_sort_err_v_is_null,

        // vec_eq
        vec_eq_ok_ordered_vectors,
        vec_eq_ok_unordered_vectors,
        vec_eq_err_v1_is_null,
        vec_eq_err_v2_is_null,
        vec_eq_err_different_sizes,

        // vec_print
        vec_print_ok,
        vec_print_err_v_is_null,
        vec_print_err_v_size_is_0,

        // vec_info
        vec_info_ok,
        vec_info_err_v_is_null,
    );
}
