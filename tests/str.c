#include "_test.h"
#include "../str.h"

// TODO free memory on tests

bool str_len_ok() {
    TEST("str_len_ok");

    ASSERT(str_len("abcde") == 5);
}

bool str_len_err_s_is_empty() {
    TEST("str_len_err_s_is_empty");

    ASSERT_ZERO(str_len(""));
}

bool str_len_err_s_is_null() {
    TEST("str_len_err_s_is_null");

    ASSERT_ZERO(str_len(NULL));
}

bool str_len_err_s_is_null_terminator() {
    TEST("str_len_err_s_is_null_terminator");

    ASSERT_ZERO(str_len("\0"));
}

bool str_clone_ok() {
    TEST("str_clone_ok");

    char *str = str_clone("string");

    ASSERT_BLOCK(
        ASSERT_STR(str, "string"),
        FREE(str),
    );
}

bool str_clone_err_s_is_empty() {
    TEST("str_clone_err_s_is_empty");

    char *str = str_clone("");

    ASSERT_BLOCK(
        ASSERT_STR(str, ""),
        FREE(str),
    );
}

bool str_clone_err_s_is_null() {
    TEST("str_clone_err_s_is_null");

    ASSERT_NULL(str_clone(NULL));
}

bool str_clone_err_s_is_null_terminator() {
    TEST("str_clone_err_s_is_null_terminator");

    char *str = str_clone("\0");

    ASSERT_BLOCK(
        ASSERT_STR(str, "\0"),
        FREE(str),
    );
}

bool str_char_at_ok_positive_index() {
    TEST("str_char_at_ok_positive_index");

    ASSERT(str_char_at("abcde", 2) == 'c');
}

bool str_char_at_ok_negative_index() {
    TEST("str_char_at_ok_negative_index");

    ASSERT(str_char_at("abcde", -2) == 'd');
}

bool str_char_at_err_positive_index_oob() {
    TEST("str_char_at_err_positive_index_oob");

    ASSERT(str_char_at("abcde", 99) == '\0');
}

bool str_char_at_err_negative_index_oob() {
    TEST("str_char_at_err_negative_index_oob");

    ASSERT(str_char_at("abcde", -99) == '\0');
}

bool __str_tok_ok() {
    TEST("__str_tok_ok");

    char *str = str_clone("ab/cd/ef");
    char *ptr = str;
    char *sep = "/";

    char *token1 = __str_tok(&str, sep);
    bool result1 = ASSERT_MANUAL(
        ASSERT_STR(token1, "ab"),
        ASSERT_STR(str, "cd/ef"),
    );

    char *token2 = __str_tok(&str, sep);
    bool result2 = ASSERT_MANUAL(
        ASSERT_STR(token2, "cd"),
        ASSERT_STR(str, "ef"),
    );

    char *token3 = __str_tok(&str, sep);
    bool result3 = ASSERT_MANUAL(
        ASSERT_STR(token3, "ef"),
        ASSERT_STR(str, ""),
    );

    ASSERT_BLOCK(
        ASSERT_TRUE(result1),
        ASSERT_TRUE(result2),
        ASSERT_TRUE(result3),

        FREE(token1),
        FREE(token2),
        FREE(token3),

        FREE(ptr),
    );
}

bool __str_tok_ok_no_separation() {
    TEST("__str_tok_ok_no_separation");

    char *str = str_clone("ab");
    char *ptr = str;
    char *sep = "/";

    char *token = __str_tok(&str, sep);

    ASSERT_BLOCK(
        ASSERT_STR(token, "ab"),
        ASSERT_STR(str, ""),

        FREE(token),
        FREE(ptr),
    );
}

bool __str_tok_ok_str_is_null() {
    TEST("__str_tok_ok_str_is_null");

    char *str = NULL;
    char *sep = "/";

    ASSERT_NULL(__str_tok(&str, sep));
}

bool __str_tok_ok_str_is_null_after_call() {
    TEST("__str_tok_ok_str_is_null_after_call");

    char *str = str_clone("ab/cd");
    char *ptr = str;
    char *sep = "/";

    char *token1 = __str_tok(&str, sep);
    char *token2 = __str_tok(&str, sep);

    ASSERT_BLOCK(
        ASSERT_STR(token1, "ab"),
        ASSERT_STR(token2, "cd"),
        ASSERT_NULL(__str_tok(&str, sep)),

        FREE(token1),
        FREE(token2),
        FREE(ptr),
    );
}

bool __str_tok_err_sep_is_null() {
    TEST("__str_tok_err_sep_is_null");

    char *str = "ab/cd/ef";
    char *sep = NULL;

    ASSERT_NULL(__str_tok(&str, sep));
}

bool str_split_ok_one_item() {
    TEST("str_split_ok_one_item");

    char *str = "abcde";
    size_t total = 0;

    char **result = str_split(str, "/", &total);
    char *expected[] = {"abcde"};
    size_t expected_size = 1;

    ASSERT_BLOCK(
        ASSERT_STR_ARR(
            result,   total,
            expected, expected_size,
        ),
        FREE_ARRAY(result, total),
    );
}

bool str_split_ok_many_items() {
    TEST("str_split_ok_many_items");

    char *str = "a/b/c/def";
    size_t total = 0;

    char **result = str_split(str, "/", &total);
    char *expected[] = {"a", "b", "c", "def"};
    size_t expected_size = 4;

    ASSERT_BLOCK(
        ASSERT_STR_ARR(
            result,     total,
            expected,   expected_size,
        ),
        FREE_ARRAY(result, total),
    );
}

bool str_split_err_str_is_null() {
    TEST("str_split_err_str_is_null");

    char *str = NULL;
    size_t total = 0;

    char **result = str_split(str, "/", &total);

    ASSERT_BLOCK(
        ASSERT_NULL(result),
        ASSERT_ZERO(total),
    );
}

bool str_split_err_sep_is_null() {
    TEST("str_split_err_sep_is_null");

    char *str = "ab/cd/ef";
    size_t total = 0;

    char **result = str_split(str, NULL, &total);

    ASSERT_BLOCK(
        ASSERT_NULL(result),
        ASSERT_ZERO(total),
    );
}

bool str_split_first_ok() {
    TEST("str_split_first_ok");

    char *result = str_split_first("a/b/c/def", "/");

    ASSERT_BLOCK(
        ASSERT_STR(result, "a"),
        FREE(result),
    );
}

bool str_split_first_err_str_is_null() {
    TEST("str_split_first_err_str_is_null");

    ASSERT_NULL(str_split_first(NULL, "/"));
}

bool str_split_first_err_no_token() {
    TEST("str_split_first_err_no_token");

    ASSERT_NULL(str_split_first("", "/"));
}

bool str_split_last_ok() {
    TEST("str_split_last_ok");

    char *result = str_split_last("a/b/c/def", "/");

    ASSERT_BLOCK(
        ASSERT_STR(result, "def"),
        FREE(result),
    );
}

bool str_split_last_err_str_is_null() {
    TEST("str_split_last_err_str_is_null");

    ASSERT_NULL(str_split_last(NULL, "/"));
}

bool str_split_last_err_no_token() {
    TEST("str_split_last_err_no_token");

    ASSERT_NULL(str_split_last("", "/"));
}

bool __str_concat_ok_heap_strings() {
    TEST("__str_concat_ok_heap_strings");

    char *s1 = str_clone("abc");
    char *s2 = str_clone("def");

    char *result = __str_concat(s1, s2);
    char *expected = "abcdef";

    ASSERT_BLOCK(
        ASSERT_STR(result, expected),

        FREE(s1),
        FREE(s2),
        FREE(result),
    );
}

bool __str_concat_ok_stack_strings() {
    TEST("__str_concat_ok_stack_strings");

    char *result = __str_concat("abc", "def");

    ASSERT_BLOCK(
        ASSERT_STR(result, "abcdef"),
        FREE(result),
    );
}

bool __str_concat_err_s1_is_null() {
    TEST("__str_concat_err_s1_is_null");

    ASSERT_NULL(__str_concat(NULL, "def"));
}

bool __str_concat_err_s2_is_null() {
    TEST("__str_concat_err_s2_is_null");

    ASSERT_NULL(__str_concat("abc", NULL));
}

bool str_concat_arr_ok() {
    TEST("str_concat_arr_ok");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 3;

    char *result = str_concat_arr(arr, size);

    ASSERT_BLOCK(
        ASSERT_STR(result, "abcdef"),
        FREE(result),
    );
}

bool str_concat_arr_err_arr_is_null() {
    TEST("str_concat_arr_err_arr_is_null");

    ASSERT_NULL(str_concat_arr(NULL, 3));
}

bool str_concat_arr_err_size_is_0() {
    TEST("str_concat_arr_err_size_is_0");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 0;

    ASSERT_NULL(str_concat_arr(arr, size));
}

bool str_concat_arr_err_arr_first_is_null() {
    TEST("str_concat_arr_err_arr_first_is_null");

    char *arr[] = {NULL, "cd", "ef"};
    size_t size = 3;

    ASSERT_NULL(str_concat_arr(arr, size));
}

bool str_concat_arr_err_arr_other_is_null() {
    TEST("str_concat_arr_err_arr_other_is_null");

    char *arr[] = {"ab", NULL, "ef"};
    size_t size = 3;

    ASSERT_NULL(str_concat_arr(arr, size));
}

bool str_concat_ok() {
    TEST("str_concat_ok");

    ASSERT_STR(str_concat("ab", "cd", "ef"), "abcdef");
}

bool str_concat_err_first_concat_err() {
    TEST("str_concat_err_first_concat_err");

    ASSERT_NULL(str_concat("ab", NULL, "ef"));
}

bool str_join_arr_ok() {
    TEST("str_join_arr_ok");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 3;

    char *result = str_join_arr("/", arr, size);
    char *expected = "ab/cd/ef";

    ASSERT_BLOCK(
        ASSERT_STR(result, expected),
        FREE(result),
    );
}

bool str_join_arr_err_joiner_is_null() {
    TEST("str_join_arr_err_joiner_is_null");

    char *arr[] = {"ab", "cd", "ef"};
    size_t size = 3;

    ASSERT_NULL(str_join_arr(NULL, arr, size));
}

bool str_join_arr_err_s1_is_null() {
    TEST("str_join_arr_err_s1_is_null");

    char *arr[] = {NULL, "cd", "ef"};
    size_t size = 3;

    ASSERT_NULL(str_join_arr("/", arr, size));
}

bool str_join_arr_err_s2_is_null() {
    TEST("str_join_arr_err_s2_is_null");

    char *arr[] = {"ab", NULL, "ef"};
    size_t size = 3;

    ASSERT_NULL(str_join_arr("/", arr, size));
}

bool str_join_ok() {
    TEST("str_join_ok");

    char *result = str_join("/", "ab", "cd", "ef");

    ASSERT_BLOCK(
        ASSERT_STR(result, "ab/cd/ef"),
        FREE(result),
    );
}

bool str_join_err_joiner_is_null() {
    TEST("str_join_err_joiner_is_null");

    ASSERT_NULL(str_join(NULL, "ab", "cd", "ef"));
}

bool str_join_err_s1_is_null() {
    TEST("str_join_err_s1_is_null");

    ASSERT_NULL(str_join("/", NULL, "cd", "ef"));
}

bool str_join_err_s2_is_null() {
    TEST("str_join_err_s2_is_null");

    ASSERT_NULL(str_join("/", "ab", NULL, "ef"));
}

bool str_ltrim_ok() {
    TEST("str_ltrim_ok");

    char *str = str_clone("  abc  ");

    ASSERT_BLOCK(
        ASSERT_STR(str_ltrim(str), "abc  "),
        FREE(str),
    );
}

bool str_ltrim_ok_s_is_empty() {
    TEST("str_ltrim_ok_s_is_empty");

    ASSERT_STR(str_ltrim(""), "");
}

bool str_ltrim_err_s_is_null() {
    TEST("str_ltrim_err_s_is_null");

    ASSERT_NULL(str_ltrim(NULL));
}

bool str_rtrim_ok() {
    TEST("str_rtrim_ok");

    char *str = str_clone("  abc  ");

    ASSERT_BLOCK(
        ASSERT_STR(str_rtrim(str), "  abc"),
        FREE(str),
    );
}

bool str_rtrim_ok_s_is_empty() {
    TEST("str_rtrim_ok_s_is_empty");

    ASSERT_STR(str_rtrim(""), "");
}

bool str_rtrim_err_s_is_null() {
    TEST("str_rtrim_err_s_is_null");

    ASSERT_NULL(str_rtrim(NULL));
}

bool str_trim_ok() {
    TEST("str_trim_ok");

    char *str = str_clone("  abc  ");

    ASSERT_BLOCK(
        ASSERT_STR(str_trim(str), "abc"),
        FREE(str),
    );
}

bool str_trim_ok_s_is_empty() {
    TEST("str_trim_ok_s_is_empty");

    ASSERT_STR(str_trim(""), "");
}

bool str_trim_err_s_is_null() {
    TEST("str_trim_err_s_is_null");

    ASSERT_NULL(str_trim(NULL));
}

bool str_eq_ok() {
    TEST("str_eq_ok");

    ASSERT_BLOCK(
        ASSERT_TRUE(str_eq("abc", "abc")),
        ASSERT_FALSE(str_eq("abc", "def")),
    );
}

bool str_eq_err_s1_is_null() {
    TEST("str_eq_err_s1_is_null");

    ASSERT_FALSE(str_eq(NULL, "abc"));
}

bool str_eq_err_s2_is_null() {
    TEST("str_eq_err_s2_is_null");

    ASSERT_FALSE(str_eq("abc", NULL));
}

bool str_diff_ok_same_len() {
    TEST("str_diff_ok_same_len");

    ASSERT_BLOCK(
        ASSERT(str_diff("", "") ==  0),
        ASSERT(str_diff("abc", "abc") ==  0),
        ASSERT(str_diff("abc", "def") == -3),
        ASSERT(str_diff("def", "abc") ==  3),
    );
}

bool str_diff_ok_s1_longer() {
    TEST("str_diff_ok_s1_longer");

    ASSERT_BLOCK(
        ASSERT(str_diff("abcd", "ef") == -4),
        ASSERT(str_diff("cdef", "ab") ==  2),
    );
}

bool str_diff_ok_s2_longer() {
    TEST("str_diff_ok_s2_longer");

    ASSERT_BLOCK(
        ASSERT(str_diff("ab", "cdef") == -2),
        ASSERT(str_diff("ef", "abcd") ==  4),
    );
}

bool str_diff_err_s1_is_null() {
    TEST("str_diff_err_s1_is_null");

    ASSERT(str_diff(NULL, "def") == -1);
}

bool str_diff_err_s2_is_null() {
    TEST("str_diff_err_s2_is_null");

    ASSERT(str_diff("abc", NULL) == -1);
}

bool str_info_ok() {
    TEST("str_info_ok");

    ASSERT_TRUE(str_info("abc"));
}

bool str_info_err_s_is_null() {
    TEST("str_info_err_s_is_null");

    ASSERT_FALSE(str_info(NULL));
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

        // __str_tok
        __str_tok_ok,
        __str_tok_ok_no_separation,
        __str_tok_ok_str_is_null,
        __str_tok_ok_str_is_null_after_call,
        __str_tok_err_sep_is_null,

        // str_split
        str_split_ok_one_item,
        str_split_ok_many_items,
        str_split_err_str_is_null,
        str_split_err_sep_is_null,

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
        str_concat_arr_err_size_is_0,
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
