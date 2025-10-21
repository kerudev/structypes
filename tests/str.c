#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../str.h"

// TODO free memory on tests
// TODO finish tests for all functions

bool str_len_ok() {
    TEST("str_len_ok");

    char *s = "abcde";

    size_t result = str_len(s);
    size_t expected = 5;

    ASSERT_BOOL(result, expected);
}

bool str_len_err_s_is_empty() {
    TEST("str_len_err_s_is_empty");

    char *s = "";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result, expected);
}

bool str_len_err_s_is_null() {
    TEST("str_len_err_s_is_null");

    char *s = NULL;

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result, expected);
}

bool str_len_err_s_is_null_terminator() {
    TEST("str_len_err_s_is_null_terminator");

    char *s = "\0";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result, expected);
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

    ASSERT_BOOL(result, expected);
}

bool str_clone_err_s_is_null_terminator() {
    TEST("str_clone_err_s_is_null_terminator");

    char *s = "\0";

    char *result = str_clone(s);
    char *expected = "\0";

    ASSERT_STR(result, expected);
}

bool str_char_at_ok_positive_index() {
    TEST("str_char_at_positive_index");

    char *s = "abcde";

    char result = str_char_at(s, 2);
    char expected = 'c';

    ASSERT_BOOL(result, expected);
}

bool str_char_at_ok_negative_index() {
    TEST("str_char_at_negative_index");

    char *s = "abcde";

    char result = str_char_at(s, -2);
    char expected = 'd';

    ASSERT_BOOL(result, expected);
}

bool str_char_at_err_positive_index_oob() {
    TEST("str_char_at_positive_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, 99);
    char expected = '\0';

    ASSERT_BOOL(result, expected);
}

bool str_char_at_err_negative_index_oob() {
    TEST("str_char_at_negative_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, -99);
    char expected = '\0';

    ASSERT_BOOL(result, expected);
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
        ASSERT_BOOL(result, expected),
        ASSERT_BOOL(total, expected_size),
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

    ASSERT_BOOL(result, expected);
}

bool str_split_first_err_no_token() {
    TEST("str_split_first_err_no_token");

    char *str = "";

    char *result = str_split_first(str, "/");
    char *expected = NULL;

    ASSERT_BOOL(result, expected);
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

    ASSERT_BOOL(result, expected);
}

bool str_split_last_err_no_token() {
    TEST("str_split_last_err_no_token");

    char *str = "";

    char *result = str_split_last(str, "/");
    char *expected = NULL;

    ASSERT_BOOL(result, expected);
}

bool str_concat_ok_heap_strings() {
    TEST("str_concat_ok");

    char *s1 = str_clone("abc");
    char *s2 = str_clone("def");

    char *result = str_concat(s1, s2);
    char *expected = "abcdef";

    free(s1);
    free(s2);

    ASSERT_STR(result, expected);
}

bool str_concat_ok_stack_strings() {
    TEST("str_concat_ok");

    char *s1 = "abc";
    char *s2 = "def";

    char *result = str_concat(s1, s2);
    char *expected = "abcdef";

    ASSERT_STR(result, expected);
}

bool str_concat_err_s1_is_null() {
    TEST("str_concat_err_s1_is_null");

    char *s1 = NULL;
    char *s2 = "def";

    char *result = str_concat(s1, s2);
    char *expected = NULL;

    ASSERT_BOOL(result, expected);
}

bool str_concat_err_s2_is_null() {
    TEST("str_concat_err_s2_is_null");

    char *s1 = "abc";
    char *s2 = NULL;

    char *result = str_concat(s1, s2);
    char *expected = NULL;

    ASSERT_BOOL(result, expected);
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

        // str_concat
        str_concat_ok_heap_strings,
        str_concat_ok_stack_strings,
        str_concat_err_s1_is_null,
        str_concat_err_s2_is_null,
    );
}
