#include "_test.h"

#define STRUCTYPES_IMPLEMENTATION
#define STRUCTYPES_DEBUG
#include "../node.h"

bool node_new_ok_null_parent_and_value() {
    TEST("node_new_ok_null_parent_and_value");

    char *value = "abcde";
    Node *node = node_new(NULL, value);

    ASSERT_BLOCK(
        ASSERT_STR(node->value, value),
        ASSERT_NULL(node->parent),
        ASSERT_NULL(node->nodes),
        ASSERT_BOOL(node->size == 0),
    );
}

bool node_new_ok_null_parent_and_null_value() {
    TEST("node_new_ok_null_parent_and_null_value");

    char *value = NULL;
    Node *node = node_new(NULL, value);

    ASSERT_BLOCK(
        ASSERT_NULL(node->value),
        ASSERT_NULL(node->parent),
        ASSERT_NULL(node->nodes),
        ASSERT_BOOL(node->size == 0),
    );
}

bool node_new_ok_with_parent_and_value() {
    TEST("node_new_ok_with_parent_and_value");

    char *value = "abcde";
    Node *parent = node_new(NULL, value);
    Node *result = node_new(parent, value);

    ASSERT_BLOCK(
        ASSERT_STR(parent->value, value),
        ASSERT_NULL(parent->parent),
        ASSERT(parent->nodes[0], result),
        ASSERT_BOOL(parent->size == 1),

        ASSERT_STR(result->value, value),
        ASSERT(result->parent, parent),
        ASSERT_NULL(result->nodes),
        ASSERT_BOOL(result->size == 0),
    );
}

bool node_new_ok_with_parent_and_null_value() {
    TEST("node_new_ok_with_parent_and_null_value");

    char *value = NULL;
    Node *parent = node_new(NULL, value);
    Node *result = node_new(parent, value);

    ASSERT_BLOCK(
        ASSERT_NULL(parent->value),
        ASSERT_NULL(parent->parent),
        ASSERT(parent->nodes[0], result),
        ASSERT_BOOL(parent->size == 1),

        ASSERT_NULL(result->value),
        ASSERT(result->parent, parent),
        ASSERT_NULL(result->nodes),
        ASSERT_BOOL(result->size == 0),
    );
}

bool nodeprintopts_default_ok() {
    TEST("nodeprintopts_default_ok");

    NodePrintOpts opts = nodeprintopts_default();

    ASSERT_BLOCK(
        ASSERT_BOOL(opts.indent == 0),
        ASSERT_BOOL(opts.indent_step == 2),
        ASSERT_BOOL(opts.sort == ASC),
    );
}

int main(int argc, char const *argv[]) {
    TEST_SUITE(
        // node_new
        node_new_ok_null_parent_and_value,
        node_new_ok_null_parent_and_null_value,
        node_new_ok_with_parent_and_value,
        node_new_ok_with_parent_and_null_value,

        // NodePrintOpts
        nodeprintopts_default_ok,
    );
}
