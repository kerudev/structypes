#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../str.h"

// TODO free memory on tests

bool str_len_ok() {
    TEST("str_len_ok");

    char *s = "abcde";

    size_t result = str_len(s);
    size_t expected = 5;

    ASSERT(result, expected);
}

bool str_len_err_s_is_empty() {
    TEST("str_len_err_s_is_empty");

    char *s = "";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT(result, expected);
}

bool str_len_err_s_is_null() {
    TEST("str_len_err_s_is_null");

    char *s = NULL;

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT(result, expected);
}

bool str_len_err_s_is_null_terminator() {
    TEST("str_len_err_s_is_null_terminator");

    char *s = "\0";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT(result, expected);
}

bool str_clone_ok() {
    TEST("str_clone_ok");

    char *s = "string";

    char *result = str_clone(s);
    char *expected = "string";

    ASSERT_STR(result, expected);
}

bool str_clone_err_s_is_empty() {
    TEST("str_clone_err_s_is_empty");

    char *s = "";

    char *result = str_clone(s);
    char *expected = "";

    ASSERT_STR(result, expected);
}

bool str_clone_err_s_is_null() {
    TEST("str_clone_err_s_is_null");

    char *s = NULL;

    char *result = str_clone(s);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_clone_err_s_is_null_terminator() {
    TEST("str_clone_err_s_is_null_terminator");

    char *s = "\0";

    char *result = str_clone(s);
    char *expected = "\0";

    ASSERT_STR(result, expected);
}

bool str_char_at_ok_positive_index() {
    TEST("str_char_at_ok_positive_index");

    char *s = "abcde";

    char result = str_char_at(s, 2);
    char expected = 'c';

    ASSERT(result, expected);
}

bool str_char_at_ok_negative_index() {
    TEST("str_char_at_ok_negative_index");

    char *s = "abcde";

    char result = str_char_at(s, -2);
    char expected = 'd';

    ASSERT(result, expected);
}

bool str_char_at_err_positive_index_oob() {
    TEST("str_char_at_err_positive_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, 99);
    char expected = '\0';

    ASSERT(result, expected);
}

bool str_char_at_err_negative_index_oob() {
    TEST("str_char_at_err_negative_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, -99);
    char expected = '\0';

    ASSERT(result, expected);
}

bool str_split_ok_one_item() {
    TEST("str_split_ok_one_item");

    char *str = "a";
    size_t total = 0;

    char **result = str_split(str, "/", &total);
    char *expected[] = {"a"};
    size_t expected_size = 1;

    ASSERT_ARR(
        result,     total,
        expected,   expected_size,
        "char*"
    );
}

bool str_split_ok_many_items() {
    TEST("str_split_ok_many_items");

    char *str = "a/b/c/def";
    size_t total = 0;

    char **result = str_split(str, "/", &total);
    char *expected[] = {"a", "b", "c", "def"};
    size_t expected_size = 4;

    ASSERT_ARR(
        result,     total,
        expected,   expected_size,
        "char*"
    );
}

bool str_split_err_str_is_null() {
    TEST("str_split_err_str_is_null");

    char *str = NULL;
    size_t total = 0;

    char **result = str_split(str, "/", &total);
    char **expected = NULL;
    size_t expected_size = 0;

    ASSERT_BLOCK(
        ASSERT(result, expected),
        ASSERT(total, expected_size),
    );
}

bool str_split_first_ok() {
    TEST("str_split_first_ok");

    char *str = "a/b/c/def";

    char *result = str_split_first(str, "/");
    char *expected = "a";

    ASSERT_STR(result, expected);
}

bool str_split_first_err_str_is_null() {
    TEST("str_split_first_err_str_is_null");

    char *str = NULL;

    char *result = str_split_first(str, "/");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_split_first_err_no_token() {
    TEST("str_split_first_err_no_token");

    char *str = "";

    char *result = str_split_first(str, "/");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_split_last_ok() {
    TEST("str_split_last_ok");

    char *str = "a/b/c/def";

    char *result = str_split_last(str, "/");
    char *expected = "def";

    ASSERT_STR(result, expected);
}

bool str_split_last_err_str_is_null() {
    TEST("str_split_last_err_str_is_null");

    char *str = NULL;

    char *result = str_split_last(str, "/");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_split_last_err_no_token() {
    TEST("str_split_last_err_no_token");

    char *str = "";

    char *result = str_split_last(str, "/");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool __str_concat_ok_heap_strings() {
    TEST("__str_concat_ok_heap_strings");

    char *s1 = str_clone("abc");
    char *s2 = str_clone("def");

    char *result = __str_concat(s1, s2);
    char *expected = "abcdef";

    free(s1);
    free(s2);

    ASSERT_STR(result, expected);
}

bool __str_concat_ok_stack_strings() {
    TEST("__str_concat_ok_stack_strings");

    char *s1 = "abc";
    char *s2 = "def";

    char *result = __str_concat(s1, s2);
    char *expected = "abcdef";

    ASSERT_STR(result, expected);
}

bool __str_concat_err_s1_is_null() {
    TEST("__str_concat_err_s1_is_null");

    char *s1 = NULL;
    char *s2 = "def";

    char *result = __str_concat(s1, s2);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool __str_concat_err_s2_is_null() {
    TEST("__str_concat_err_s2_is_null");

    char *s1 = "abc";
    char *s2 = NULL;

    char *result = __str_concat(s1, s2);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_concat_arr_ok() {
    TEST("str_concat_arr_ok");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 3;

    char *result = str_concat_arr(arr, size);
    char *expected = "abcdef";

    ASSERT_STR(result, expected);
}

bool str_concat_arr_err_arr_is_null() {
    TEST("str_concat_arr_err_arr_is_null");

    char **arr = NULL;
    size_t size = 3;

    char *result = str_concat_arr(arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_concat_arr_err_size_is_zero() {
    TEST("str_concat_arr_err_size_is_zero");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 0;

    char *result = str_concat_arr(arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_concat_arr_err_arr_first_is_null() {
    TEST("str_concat_arr_err_arr_first_is_null");

    char *arr[] = {NULL, "cd", "ef"};
    size_t size = 3;

    char *result = str_concat_arr(arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_concat_arr_err_arr_other_is_null() {
    TEST("str_concat_arr_err_arr_other_is_null");

    char *arr[] = {"ab", NULL, "ef"};
    size_t size = 3;

    char *result = str_concat_arr(arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_concat_ok() {
    TEST("str_concat_ok");

    char *result = str_concat("ab", "cd", "ef");
    char *expected = "abcdef";

    ASSERT_STR(result, expected);
}

bool str_concat_err_first_concat_err() {
    TEST("str_concat_err_first_concat_err");

    char *result = str_concat("ab", NULL, "ef");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_arr_ok() {
    TEST("str_join_arr_ok");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 3;

    char *result = str_join_arr("/", arr, size);
    char *expected = "ab/cd/ef";

    ASSERT_STR(result, expected);
}

bool str_join_arr_err_joiner_is_null() {
    TEST("str_join_arr_err_joiner_is_null");

    char *arr[] = { "ab", "cd", "ef"};
    size_t size = 3;

    char *result = str_join_arr(NULL, arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_arr_err_s1_is_null() {
    TEST("str_join_arr_err_s1_is_null");

    char *arr[] = {NULL, "cd", "ef"};
    size_t size = 3;

    char *result = str_join_arr("/", arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_arr_err_s2_is_null() {
    TEST("str_join_arr_err_s2_is_null");

    char *arr[] = {"ab", NULL, "ef"};
    size_t size = 3;

    char *result = str_join_arr("/", arr, size);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_ok() {
    TEST("str_join_ok");

    char *result = str_join("/", "ab", "cd", "ef");
    char *expected = "ab/cd/ef";

    ASSERT_STR(result, expected);
}

bool str_join_err_joiner_is_null() {
    TEST("str_join_err_joiner_is_null");

    char *result = str_join(NULL, "ab", "cd", "ef");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_err_s1_is_null() {
    TEST("str_join_err_s1_is_null");

    char *result = str_join("/", NULL, "cd", "ef");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_join_err_s2_is_null() {
    TEST("str_join_err_s2_is_null");

    char *result = str_join("/", "ab", NULL, "ef");
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_ltrim_ok() {
    TEST("str_ltrim_ok");

    char *s = str_clone("  abc  ");

    char *result = str_ltrim(s);
    char *expected = "abc  ";

    ASSERT_STR(result, expected);
}

bool str_ltrim_ok_s_is_empty() {
    TEST("str_ltrim_ok_s_is_empty");

    char *result = str_ltrim("");
    char *expected = "";

    ASSERT_STR(result, expected);
}

bool str_ltrim_err_s_is_null() {
    TEST("str_ltrim_err_s_is_null");

    char *result = str_ltrim(NULL);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_rtrim_ok() {
    TEST("str_rtrim_ok");

    char *s = str_clone("  abc  ");

    char *result = str_rtrim(s);
    char *expected = "  abc";

    ASSERT_STR(result, expected);
}

bool str_rtrim_ok_s_is_empty() {
    TEST("str_rtrim_ok_s_is_empty");

    char *result = str_rtrim("");
    char *expected = "";

    ASSERT_STR(result, expected);
}

bool str_rtrim_err_s_is_null() {
    TEST("str_rtrim_err_s_is_null");

    char *result = str_rtrim(NULL);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_trim_ok() {
    TEST("str_trim_ok");

    char *s = str_clone("  abc  ");

    char *result = str_trim(s);
    char *expected = "abc";

    ASSERT_STR(result, expected);
}

bool str_trim_ok_s_is_empty() {
    TEST("str_trim_ok_s_is_empty");

    char *result = str_trim("");
    char *expected = "";

    ASSERT_STR(result, expected);
}

bool str_trim_err_s_is_null() {
    TEST("str_trim_err_s_is_null");

    char *result = str_trim(NULL);
    char *expected = NULL;

    ASSERT(result, expected);
}

bool str_eq_ok() {
    TEST("str_eq_ok");

    ASSERT_BLOCK(
        ASSERT_BOOL(str_eq("abc", "abc")),
        ASSERT_BOOL(!str_eq("abc", "def")),
    );
}

bool str_eq_err_s1_is_null() {
    TEST("str_eq_err_s1_is_null");

    ASSERT_BOOL(!str_eq(NULL, "abc"));
}

bool str_eq_err_s2_is_null() {
    TEST("str_eq_err_s2_is_null");

    ASSERT_BOOL(!str_eq("abc", NULL));
}

bool str_diff_ok_same_len() {
    TEST("str_diff_ok_same_len");

    ASSERT_BLOCK(
        ASSERT_BOOL(str_diff("", "") ==  0),
        ASSERT_BOOL(str_diff("abc", "abc") ==  0),
        ASSERT_BOOL(str_diff("abc", "def") == -3),
        ASSERT_BOOL(str_diff("def", "abc") ==  3),
    );
}

bool str_diff_ok_s1_longer() {
    TEST("str_diff_ok_s1_longer");

    ASSERT_BLOCK(
        ASSERT_BOOL(str_diff("abcd", "ef") == -4),
        ASSERT_BOOL(str_diff("cdef", "ab") ==  2),
    );
}

bool str_diff_ok_s2_longer() {
    TEST("str_diff_ok_s2_longer");

    ASSERT_BLOCK(
        ASSERT_BOOL(str_diff("ab", "cdef") == -2),
        ASSERT_BOOL(str_diff("ef", "abcd") ==  4),
    );
}

bool str_diff_err_s1_is_null() {
    TEST("str_diff_err_s1_is_null");

    int result = str_diff(NULL, "def");
    int expected = -1;

    ASSERT(result, expected);
}

bool str_diff_err_s2_is_null() {
    TEST("str_diff_err_s2_is_null");

    int result = str_diff("abc", NULL);
    int expected = -1;

    ASSERT(result, expected);
}

bool str_info_ok() {
    TEST("str_info_ok");

    ASSERT_BOOL(str_info("abc"));
}

bool str_info_err_s_is_null() {
    TEST("str_info_err_s_is_null");

    ASSERT_BOOL(!str_info(NULL));
}

int main() {
    TEST_SUITE(
        // str_len
        str_len_ok,
        str_len_err_s_is_empty,
        str_len_err_s_is_null,
        str_len_err_s_is_null_terminator,

        // str_clone
        str_clone_ok,
        str_clone_err_s_is_empty,
        str_clone_err_s_is_null,
        str_clone_err_s_is_null_terminator,

        // str_char_at
        str_char_at_ok_positive_index,
        str_char_at_ok_negative_index,
        str_char_at_err_positive_index_oob,
        str_char_at_err_negative_index_oob,

        // str_split
        str_split_ok_one_item,
        str_split_ok_many_items,
        str_split_err_str_is_null,

        // str_split_first
        str_split_first_ok,
        str_split_first_err_str_is_null,
        str_split_first_err_no_token,

        // str_split_last
        str_split_last_ok,
        str_split_last_err_str_is_null,
        str_split_last_err_no_token,

        // __str_concat
        __str_concat_ok_heap_strings,
        __str_concat_ok_stack_strings,
        __str_concat_err_s1_is_null,
        __str_concat_err_s2_is_null,

        // str_concat_arr
        str_concat_arr_ok,
        str_concat_arr_err_arr_is_null,
        str_concat_arr_err_size_is_zero,
        str_concat_arr_err_arr_first_is_null,
        str_concat_arr_err_arr_other_is_null,

        // str_join_arr
        str_join_arr_ok,
        str_join_arr_err_joiner_is_null,
        str_join_arr_err_s1_is_null,
        str_join_arr_err_s2_is_null,

        // str_join
        str_join_ok,
        str_join_err_joiner_is_null,
        str_join_err_s1_is_null,
        str_join_err_s2_is_null,

        // str_ltrim
        str_ltrim_ok,
        str_ltrim_ok_s_is_empty,
        str_ltrim_err_s_is_null,

        // str_rtrim
        str_rtrim_ok,
        str_rtrim_ok_s_is_empty,
        str_rtrim_err_s_is_null,

        // str_trim
        str_trim_ok,
        str_trim_ok_s_is_empty,
        str_trim_err_s_is_null,

        // str_eq
        str_eq_ok,
        str_eq_err_s1_is_null,
        str_eq_err_s2_is_null,

        // str_diff
        str_diff_ok_same_len,
        str_diff_ok_s1_longer,
        str_diff_ok_s2_longer,
        str_diff_err_s1_is_null,
        str_diff_err_s2_is_null,

        // str_info
        str_info_ok,
        str_info_err_s_is_null,
    );
}
