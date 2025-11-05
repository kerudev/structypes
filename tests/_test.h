#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define STRUCTYPES_IMPLEMENTATION
#include "../str.h"
#include "../vec.h"

static char *current_test = NULL;
static Vec *failed_tests = NULL;
static bool in_assert_block = false;

void TEST(const char *name) {
    current_test = str_clone(name);
}

void _test_ok() {
    printf("[OK ] %s\n", current_test);
    free(current_test);
}

void _test_err(const char *msg, ...) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "[ERR] %s: ", current_test);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);

    vec_push(failed_tests, str_clone(current_test));
}

void _test_skip() {
    fprintf(stderr, "[ X ] %s: test skipped\n", current_test);
}

bool _assert(bool b, ...) {
    if (!b) {
        va_list args;
        va_start(args, b);

        char *msg = va_arg(args, char *);
        if (!msg) msg = str_clone("assertion failed");
        _test_err(msg, args);

        va_end(args);
    }

    return b;
}

bool _assert_block(bool *results, size_t total) {
    for (size_t i = 0; i < total; i++)
        if (!results[i]) {
            _test_err("assertion %zu", i);
            return false;
        }

    return true;
}

bool _assert_arr_str(char **a1, size_t s1, char **a2, size_t s2) {
    if (s1 != s2) {
        _test_err("array sizes are different");
        _test_err("s1 = %zu, s2 = %zu", s1, s2);
        return false;
    }

    for (size_t i = 0; i < s1; i++) {
        if (!str_eq(a1[i], a2[i])) {
            _test_err("elements at index %zu are different", i);
            _test_err("a1[%zu] = %s, a2[%zu] = %s", i, a1[i], i, a2[i]);
            return false;
        }
    }

    return true;
}

bool _assert_arr_int(int a1[], size_t s1, int a2[], size_t s2) {
    if (s1 != s2) {
        _test_err("array sizes are different");
        _test_err("s1 = %zu, s2 = %zu",s1, s2);
        return false;
    }

    for (size_t i = 0; i < s1; i++) {
        if (a1[i] != a2[i]) {
            _test_err("elements at index %zu are different", i);
            _test_err("a1[%zu] = %i, a2[%zu] = %i", i, a1[i], i, a2[i]);
            return false;
        }
    }

    return true;
}

bool _assert_arr(void *a1, size_t s1, void *a2, size_t s2, char *type) {
    if (s1 != s2) {
        _test_err("array sizes are different");
        _test_err("s1 = %zu, s2 = %zu", s1, s2);
        return false;
    }

    return (str_eq(type, "char*"))
        ? _assert_arr_str(a1, s1, a2, s2)
        : _assert_arr_int(a1, s1, a2, s2);
}

int _test_suite(bool (**tests)(), size_t total) {
    failed_tests = vec_new(sizeof(char *));

    printf("running tests (%zu total)\n", total);
    printf("--------------------------------\n");

    for (size_t i = 0; i < total; i++) {
        if (!tests[i]()) return 1;
        _test_ok();
    }

    printf("--------------------------------\n");
    if (!failed_tests->size) {
        printf("all tests passed\n");
    } else {
        printf("the following tests failed\n");
        vec_print(failed_tests);
    }

    return 0;
}

#define ASSERT(b, ...) ({ \
    bool ok = _assert(b, ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_ZERO(n, ...) ({ \
    bool ok = _assert(n == 0, ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_TRUE(b, ...) ({ \
    bool ok = _assert(b, ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_FALSE(b, ...) ({ \
    bool ok = _assert(!b, ##__VA_ARGS__);   \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_NULL(cond, ...) ({ \
    bool ok = _assert((cond == NULL), ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_NOT_NULL(cond, ...) ({ \
    bool ok = _assert((cond != NULL), ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_STR(s1, s2, ...) ({ \
    bool ok = _assert(str_eq(s1, s2), ##__VA_ARGS__); \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_ARR(a1, s1, a2, s2, type, ...) ({ \
    bool ok = _assert_arr(a1, s1, a2, s2, type, ##__VA_ARGS__); \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_BLOCK(...) ({                \
    in_assert_block = true;                 \
    bool results[] = { __VA_ARGS__ };       \
    bool ok = _assert_block(results, sizeof(results)/sizeof(results[0])); \
    in_assert_block = false;                \
    return ok;                              \
})

#define SKIP() {    \
    _test_skip();   \
    return true;    \
}

#define TEST_SUITE(...) ({                      \
    bool (*tests[])(void) = { __VA_ARGS__ };    \
    return _test_suite(tests, sizeof(tests)/sizeof(tests[0])); \
})
