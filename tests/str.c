#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../str.h"

bool str_len_ok() {
    TEST("str_len_ok");

    char *s = "abcde";

    size_t result = str_len(s);
    size_t expected = 5;

    ASSERT_BOOL(result == expected);
}

bool str_len_err_s_is_empty() {
    TEST("str_len_err_s_is_empty");

    char *s = "";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result == expected);
}

bool str_len_err_s_is_null() {
    TEST("str_len_err_s_is_null");

    char *s = NULL;

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result == expected);
}

bool str_len_err_s_is_null_terminator() {
    TEST("str_len_err_s_is_null_terminator");

    char *s = "\0";

    size_t result = str_len(s);
    size_t expected = 0;

    ASSERT_BOOL(result == expected);
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

    ASSERT_BOOL(result == expected);
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

    ASSERT_BOOL(result == expected);
}

bool str_char_at_ok_negative_index() {
    TEST("str_char_at_negative_index");

    char *s = "abcde";

    char result = str_char_at(s, -2);
    char expected = 'd';

    ASSERT_BOOL(result == expected);
}

bool str_char_at_err_positive_index_oob() {
    TEST("str_char_at_positive_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, 99);
    char expected = '\0';

    ASSERT_BOOL(result == expected);
}

bool str_char_at_err_negative_index_oob() {
    TEST("str_char_at_negative_index_oob");

    char *s = "abcde";

    char result = str_char_at(s, -99);
    char expected = '\0';

    ASSERT_BOOL(result == expected);
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
    char *expected = NULL;
    size_t expected_size = 0;

    ASSERT_BLOCK(
        ASSERT_BOOL(result == expected),
        ASSERT_BOOL(total == expected_size),
    );
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
    );
}
