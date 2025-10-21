#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "../str.h"

static char *current_test = NULL;
static bool in_assert_block = false;

static void TEST(const char *name) {
    if (current_test) free(current_test);
    current_test = str_clone(name);
}

static void _test_ok() {
    printf("[OK ] %s\n", current_test);
}

static void _test_err() {
    fprintf(stderr, "[ERR] %s\n", current_test);
}

static bool _assert(bool b, ...) {
    if (!b) {
        va_list args;
        va_start(args, b);

        char *msg = va_arg(args, char *);

        if (msg) {
            vfprintf(stderr, msg, args);
            fprintf(stderr, "\n");
        } else {
            _test_err();
        }

        va_end(args);
    }

    return b;
}

static bool _assert_block(bool *results, size_t total) {
    for (size_t i = 0; i < total; i++)
        if (!results[i]) {
            fprintf(stderr, "[ERR] %s: assertion %zu\n", current_test, i);
            return false;
        }

    return true;
}

static bool _assert_arr_str(char **a1, size_t s1, char **a2, size_t s2) {
    if (s1 != s2) {
        fprintf(stderr, "[ERR] %s: array sizes are different\n", current_test);
        fprintf(stderr, "[ERR] %s: s1 = %zu, s2 = %zu \n", current_test, s1, s2);
    }

    for (size_t i = 0; i < s1; i++) {
        if (strcmp(a1[i], a2[i]) != 0) {
            fprintf(stderr, "[ERR] %s: elements at index %zu are different\n", current_test, i);
            fprintf(stderr, "[ERR] %s: a1[%zu] = %s, a2[%zu] = %s\n", current_test, i, a1[i], i, a2[i]);
            return false;
        }
    }

    return true;
}

static bool _assert_arr_int(int a1[], size_t s1, int a2[], size_t s2) {
    if (s1 != s2) {
        fprintf(stderr, "[ERR] %s: array sizes are different\n", current_test);
        fprintf(stderr, "[ERR] %s: s1 = %zu, s2 = %zu \n", current_test, s1, s2);
    }

    for (size_t i = 0; i < s1; i++) {
        if (a1[i] != a2[i]) {
            fprintf(stderr, "[ERR] %s: elements at index %zu are different\n", current_test, i);
            fprintf(stderr, "[ERR] %s: a1[%zu] = %i, a2[%zu] = %i\n", current_test, i, a1[i], i, a2[i]);
            return false;
        }
    }

    return true;
}

static bool _assert_arr(void *a1, size_t s1, void *a2, size_t s2, char *type) {
    if (s1 != s2) {
        fprintf(stderr, "[ERR] %s: array sizes are different\n", current_test);
        fprintf(stderr, "[ERR] %s: s1 = %zu, s2 = %zu \n", current_test, s1, s2);
    }

    (strcmp(type, "char*") == 0) 
        ? _assert_arr_str(a1, s1, a2, s2)
        : _assert_arr_int(a1, s1, a2, s2);
}

static int _test_suite(bool (**tests)(), size_t total) {
    for (size_t i = 0; i < total; i++) {
        // printf("%d\n", i);
        if (!tests[i]()) return 1;
        _test_ok();
    }

    printf("all tests passed\n");
    return 0;
}

#define ASSERT_BOOL(b, ...) ({ \
    bool ok = _assert(b, ##__VA_ARGS__);    \
    if (!in_assert_block) return ok;        \
    ok;                                     \
})

#define ASSERT_STR(s1, s2, ...) ({ \
    bool ok = _assert(!strcmp(s1, s2), ##__VA_ARGS__); \
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

#define TEST_SUITE(...) ({                      \
    bool (*tests[])(void) = { __VA_ARGS__ };    \
    return _test_suite(tests, sizeof(tests)/sizeof(tests[0])); \
})
