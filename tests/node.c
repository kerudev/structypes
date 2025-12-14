#include "_test.h"
#include "../includes/node.h"

bool node_new_ok_heap_allocated() {
    TEST("node_new_ok_heap_allocated");

    char *value = str_clone("abcde");
    Node *node = node_new(value);

    ASSERT_BLOCK(
        ASSERT_STR(node->value, value),
        ASSERT_NULL(node->parent),
        ASSERT_NULL(node->nodes),
        ASSERT_ZERO(node->size),

        ASSERT_TRUE(node_free(node)),
    );
}

bool node_new_ok_stack_allocated() {
    TEST("node_new_ok_stack_allocated");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_STR(node->value, "abcde"),
        ASSERT_NULL(node->parent),
        ASSERT_NULL(node->nodes),
        ASSERT_ZERO(node->size),

        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_new_ok_value_is_null() {
    TEST("node_new_ok_value_is_null");

    Node *node = node_new(NULL);

    ASSERT_BLOCK(
        ASSERT_NULL(node->value),
        ASSERT_NULL(node->parent),
        ASSERT_NULL(node->nodes),
        ASSERT_ZERO(node->size),

        ASSERT_TRUE(node_free(node)),
    );
}

bool node_add_ok_heap_allocated() {
    TEST("node_add_ok_heap_allocated");

    char *value1 = str_clone("abcde");
    char *value2 = str_clone("12345");

    Node *parent = node_new(value1);
    Node *child = node_add(parent, value2);

    ASSERT_BLOCK(
        ASSERT_STR(parent->value, value1),
        ASSERT_NULL(parent->parent),
        ASSERT_TRUE(parent->nodes[0] == child),
        ASSERT_TRUE(parent->size == 1),

        ASSERT_STR(child->value, value2),
        ASSERT_TRUE(child->parent == parent),
        ASSERT_NULL(child->nodes),
        ASSERT_ZERO(child->size),

        ASSERT_TRUE(node_free(parent)),
    );
}

bool node_add_ok_stack_allocated() {
    TEST("node_add_ok_stack_allocated");

    Node *parent = node_new("abcde");
    Node *child = node_add(parent, "12345");

    ASSERT_BLOCK(
        ASSERT_STR(parent->value, "abcde"),
        ASSERT_NULL(parent->parent),
        ASSERT_TRUE(parent->nodes[0] == child),
        ASSERT_TRUE(parent->size == 1),

        ASSERT_STR(child->value, "12345"),
        ASSERT_TRUE(child->parent == parent),
        ASSERT_NULL(child->nodes),
        ASSERT_ZERO(child->size),

        ASSERT_TRUE(node_free_stack(parent)),
    );
}

bool node_add_ok_value_is_null() {
    TEST("node_add_ok_value_is_null");

    Node *parent = node_new(NULL);
    Node *child = node_add(parent, NULL);

    ASSERT_BLOCK(
        ASSERT_NULL(parent->value),
        ASSERT_NULL(parent->parent),
        ASSERT_TRUE(parent->nodes[0] == child),
        ASSERT_TRUE(parent->size == 1),

        ASSERT_NULL(child->value),
        ASSERT_TRUE(child->parent == parent),
        ASSERT_NULL(child->nodes),
        ASSERT_ZERO(child->size),

        ASSERT_TRUE(node_free(parent)),
    );
}

bool node_add_err_parent_is_null() {
    TEST("node_add_err_parent_is_null");

    ASSERT_NULL(node_add(NULL, "abcde"));
}

bool node_add_child_ok_heap_allocated() {
    TEST("node_add_child_ok_heap_allocated");

    char *value1 = str_clone("abcde");
    char *value2 = str_clone("12345");

    Node *parent = node_new(value1);
    Node *child = node_new(value2);

    ASSERT_BLOCK(
        ASSERT_TRUE(node_add_child(parent, child)),

        ASSERT_STR(parent->value, value1),
        ASSERT_NULL(parent->parent),
        ASSERT_TRUE(parent->nodes[0] == child),
        ASSERT_TRUE(parent->size == 1),

        ASSERT_STR(child->value, value2),
        ASSERT_TRUE(child->parent == parent),
        ASSERT_NULL(child->nodes),
        ASSERT_ZERO(child->size),

        ASSERT_TRUE(node_free(parent)),
    );
}

bool node_add_child_ok_stack_allocated() {
    TEST("node_add_child_ok_stack_allocated");

    Node *parent = node_new("abcde");
    Node *child = node_new("12345");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_add_child(parent, child)),

        ASSERT_STR(parent->value, "abcde"),
        ASSERT_NULL(parent->parent),
        ASSERT_TRUE(parent->nodes[0] == child),
        ASSERT_TRUE(parent->size == 1),

        ASSERT_STR(child->value, "12345"),
        ASSERT_TRUE(child->parent == parent),
        ASSERT_NULL(child->nodes),
        ASSERT_ZERO(child->size),

        ASSERT_TRUE(node_free_stack(parent)),
    );
}

bool node_add_child_err_parent_is_null() {
    TEST("node_add_child_err_parent_is_null");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_FALSE(node_add_child(NULL, node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_add_child_err_child_is_null() {
    TEST("node_add_child_err_child_is_null");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_FALSE(node_add_child(node, NULL)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_add_child_err_parent_eq_child() {
    TEST("node_add_child_err_parent_eq_child");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_FALSE(node_add_child(node, node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_print_ok() {
    TEST("node_print_ok");

    Node *parent = node_new("abcde");
    node_add(parent, "12345");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_print(parent, nodeprintopts_default())),
        ASSERT_TRUE(node_free_stack(parent)),
    );
}

bool node_print_ok_node_size_is_0() {
    TEST("node_print_ok_node_size_is_0");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_print(node, nodeprintopts_default())),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_print_err_node_is_null() {
    TEST("node_print_err_node_is_null");

    ASSERT_FALSE(node_print(NULL, (NodePrintOpts){0}));
}

bool node_print_err_opts_indent_step_lt_1() {
    TEST("node_print_err_opts_indent_step_lt_1");

    Node *node = node_new("abcde");
    NodePrintOpts opts = nodeprintopts_default();
    opts.indent_step = 0;

    ASSERT_BLOCK(
        ASSERT_FALSE(node_print(node, opts)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_print_deep_ok() {
    TEST("node_print_deep_ok");

    Node *node = node_new("abcde");
    node_add(node, "12345");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_print_deep(node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_print_deep_err_node_is_null() {
    TEST("node_print_deep_err_node_is_null");

    ASSERT_FALSE(node_print_deep(NULL));
}

bool node_print_shallow_ok() {
    TEST("node_print_shallow_ok");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_print_shallow(node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_print_shallow_err_node_is_null() {
    TEST("node_print_shallow_err_node_is_null");

    ASSERT_FALSE(node_print_shallow(NULL));
}

bool node_info_ok() {
    TEST("node_info_ok");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_info(node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_info_err_node_is_null() {
    TEST("node_info_err_node_is_null");

    ASSERT_FALSE(node_info(NULL));
}

bool node_total_ok() {
    TEST("node_total_ok");

    Node *parent = node_new("abcde");
    node_add(parent, "123");
    node_add(parent, "456");

    ASSERT_BLOCK(
        ASSERT_TRUE(node_total(parent) == 2),
        ASSERT_TRUE(node_free_stack(parent)),
    );
}

bool node_total_ok_without_nodes() {
    TEST("node_total_ok_without_nodes");

    Node *node = node_new("abcde");

    ASSERT_BLOCK(
        ASSERT_ZERO(node_total(node)),
        ASSERT_TRUE(node_free_stack(node)),
    );
}

bool node_total_err_node_is_null() {
    TEST("node_total_err_node_is_null");

    ASSERT_ZERO(node_total(NULL));
}

bool node_free_struct_ok() {
    TEST("node_free_struct_ok");

    Node *node = node_new("abcde");

    ASSERT_TRUE(node_free_struct(node));
}

bool node_free_struct_ok_value_is_null() {
    TEST("node_free_struct_ok_value_is_null");

    Node *node = node_new(NULL);

    ASSERT_TRUE(node_free_struct(node));
}

bool node_free_struct_ok_nodes_is_null() {
    TEST("node_free_struct_ok_nodes_is_null");

    Node *node = node_new("abcde");

    free(node->nodes);
    node->nodes = NULL;

    ASSERT_TRUE(node_free_struct(node));
}

bool node_free_struct_err_node_is_null() {
    TEST("node_free_struct_err_node_is_null");

    ASSERT_FALSE(node_free_struct(NULL));
}

bool node_free_ok() {
    TEST("node_free_ok");

    Node *parent = node_new(str_clone("abcde"));
    node_add(parent, str_clone("123"));
    node_add(parent, str_clone("456"));

    ASSERT_TRUE(node_free(parent));
}

bool node_free_ok_no_nodes() {
    TEST("node_free_ok_no_nodes");

    Node *node = node_new(str_clone("abcde"));
    node->size = 1;

    ASSERT_TRUE(node_free(node));
}

bool node_free_err_node_is_null() {
    TEST("node_free_err_node_is_null");

    ASSERT_FALSE(node_free(NULL));
}

bool node_free_stack_ok() {
    TEST("node_free_stack_ok");

    Node *node = node_new("parent");
    node_add(node, "1");
    node_add(node, "2");
    node_add(node, "3");

    ASSERT_TRUE(node_free_stack(node));
}

bool node_free_stack_ok_no_nodes() {
    TEST("node_free_stack_ok_no_nodes");

    Node *node = node_new("abcde");

    ASSERT_TRUE(node_free_stack(node));
}

bool node_free_stack_err_node_is_null() {
    TEST("node_free_stack_err_node_is_null");

    ASSERT_FALSE(node_free_stack(NULL));
}

bool nodeprintopts_default_ok() {
    TEST("nodeprintopts_default_ok");

    NodePrintOpts opts = nodeprintopts_default();

    ASSERT_BLOCK(
        ASSERT_ZERO(opts.indent),
        ASSERT_TRUE(opts.indent_step == 2),
        ASSERT_TRUE(opts.sort == ASC),
    );
}

int main() {
    TEST_SUITE(
        // node_new
        node_new_ok_heap_allocated,
        node_new_ok_stack_allocated,
        node_new_ok_value_is_null,

        // node_add
        node_add_ok_heap_allocated,
        node_add_ok_stack_allocated,
        node_add_ok_value_is_null,
        node_add_err_parent_is_null,

        // node_add_child
        node_add_child_ok_heap_allocated,
        node_add_child_ok_stack_allocated,
        node_add_child_err_parent_is_null,
        node_add_child_err_child_is_null,
        node_add_child_err_parent_eq_child,

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

        // node_free_struct
        node_free_struct_ok,
        node_free_struct_ok_value_is_null,
        node_free_struct_ok_nodes_is_null,
        node_free_struct_err_node_is_null,

        // node_free
        node_free_ok,
        node_free_ok_no_nodes,
        node_free_err_node_is_null,

        // node_free_stack
        node_free_stack_ok,
        node_free_stack_ok_no_nodes,
        node_free_stack_err_node_is_null,

        // NodePrintOpts
        nodeprintopts_default_ok,
    );
}
