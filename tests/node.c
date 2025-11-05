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

bool node_add_ok() {
    TEST("node_add_ok");

    char *value1 = "abcde";
    char *value2 = "12345";

    Node *parent = node_new(NULL, value1);
    Node *child = node_new(NULL, value2);

    ASSERT_BLOCK(
        ASSERT_TRUE(node_add(parent, child)),

        ASSERT_STR(parent->value, value1),
        ASSERT_NULL(parent->parent),
        ASSERT(parent->nodes[0], child),
        ASSERT_BOOL(parent->size == 1),

        ASSERT_STR(child->value, value2),
        ASSERT(child->parent, parent),
        ASSERT_NULL(child->nodes),
        ASSERT_BOOL(child->size == 0),
    );
}

bool node_add_err_parent_is_null() {
    TEST("node_add_err_parent_is_null");

    char *value = "abcde";
    Node *child = node_new(NULL, value);

    ASSERT_FALSE(node_add(NULL, child));
}

bool node_add_err_child_is_null() {
    TEST("node_add_err_child_is_null");

    char *value = "abcde";
    Node *parent = node_new(NULL, value);

    ASSERT_FALSE(node_add(parent, NULL));
}

bool node_print_ok() {
    TEST("node_print_ok");

    char *value1 = "abcde";
    char *value2 = "12345";

    Node *parent = node_new(NULL, value1);
    Node *child = node_new(parent, value2);

    ASSERT_TRUE(node_print(parent, nodeprintopts_default()));
}

bool node_print_ok_node_size_is_0() {
    TEST("node_print_ok_node_size_is_0");

    Node *node = node_new(NULL, "abcde");

    ASSERT_TRUE(node_print(node, nodeprintopts_default()));
}

bool node_print_err_node_is_null() {
    TEST("node_print_err_node_is_null");

    ASSERT_FALSE(node_print(NULL, (NodePrintOpts){0}));
}

bool node_print_err_opts_indent_step_lt_1() {
    TEST("node_print_err_opts_indent_step_lt_1");

    Node *node = node_new(NULL, "abcde");
    NodePrintOpts opts = nodeprintopts_default();
    opts.indent_step = 0;

    ASSERT_FALSE(node_print(node, opts));
}

bool node_print_deep_ok() {
    TEST("node_print_deep_ok");

    char *value1 = str_clone("abcde");
    char *value2 = str_clone("12345");

    Node *parent = node_new(NULL, value1);
    Node *child = node_new(parent, value2);

    ASSERT_TRUE(node_print_deep(parent));
}

bool node_print_deep_err_node_is_null() {
    TEST("node_print_deep_err_node_is_null");

    ASSERT_FALSE(node_print_deep(NULL));
}

bool node_print_shallow_ok() {
    TEST("node_print_shallow_ok");

    Node *node = node_new(NULL, "abcde");

    ASSERT_TRUE(node_print_shallow(node));
}

bool node_print_shallow_err_node_is_null() {
    TEST("node_print_shallow_err_node_is_null");

    ASSERT_FALSE(node_print_shallow(NULL));
}

bool node_info_ok() {
    TEST("node_info_ok");

    ASSERT_TRUE(node_info(node_new(NULL, "abcde")));
}

bool node_info_err_node_is_null() {
    TEST("node_info_err_node_is_null");

    ASSERT_FALSE(node_info(NULL));
}

bool node_total_ok() {
    TEST("node_total_ok");

    Node *parent = node_new(NULL, "abcde");
    Node *child1 = node_new(parent, "123");
    Node *child2 = node_new(parent, "456");

    ASSERT_BOOL(node_total(parent) == 2);
}

bool node_total_ok_without_nodes() {
    TEST("node_total_ok_without_nodes");

    ASSERT_ZERO(node_total(node_new(NULL, "abcde")));
}

bool node_total_err_node_is_null() {
    TEST("node_total_err_node_is_null");

    ASSERT_ZERO(node_total(NULL));
}

bool node_free_ok() {
    TEST("node_free_ok");

    Node *parent = node_new(NULL, str_clone("abcde"));
    Node *child1 = node_new(parent, str_clone("123"));
    Node *child2 = node_new(parent, str_clone("456"));

    ASSERT_TRUE(node_free(parent));
}

bool node_free_err_node_is_null() {
    TEST("node_free_err_node_is_null");

    ASSERT_FALSE(node_free(NULL));
}

bool node_free_err_no_nodes() {
    TEST("node_free_err_no_nodes");

    Node *node = node_new(NULL, "abcde");
    node->size = 1;

    ASSERT_FALSE(node_free(node));
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

        // node_add
        node_add_ok,
        node_add_err_parent_is_null,
        node_add_err_child_is_null,

        // node_print
        node_print_ok,
        node_print_ok_node_size_is_0,
        node_print_err_node_is_null,
        node_print_err_opts_indent_step_lt_1,

        // node_print_deep
        node_print_deep_ok,
        node_print_deep_err_node_is_null,

        // node_print_shallow
        node_print_shallow_ok,
        node_print_shallow_err_node_is_null,

        // node_info
        node_info_ok,
        node_info_err_node_is_null,

        // node_total
        node_total_ok,
        node_total_ok_without_nodes,
        node_total_err_node_is_null,

        // node_free
        node_free_ok,
        node_free_err_node_is_null,
        node_free_err_no_nodes,

        // NodePrintOpts
        nodeprintopts_default_ok,
    );
}
