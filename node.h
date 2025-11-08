// node.h - A node implementation.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - STRUCTYPES_DEBUG: if defined, prints error messages.
// - NODE_IMPLEMENTATION: implementations of node functions.

#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

/** A node. */
typedef struct Node {
    /** Value of the node. */
    void *value;
    /** Pointer to the parent node. */
    struct Node *parent;
    /** Array of children nodes. */
    struct Node **nodes;
    /** Number of child nodes. */
    size_t size;
} Node;

/**
 * Defines how a function should handle sorting algorithms:
 * - `NONE`: insertion order (no sort).
 * - `ASC` : ascending order.
 * - `DESC`: descending order.
 */
typedef enum {
    NONE,
    ASC,
    DESC,
} NodeSortOpts;

/**
 * Defines how nodes will be printed:
 * - indent: starting indentation.
 * - indent_step: indent increment on each level.
 * - sort: how nodes' values will be sorted. Check docs for `NodeSortOpts`.
 */
typedef struct {
    unsigned int indent;
    unsigned int indent_step;
    NodeSortOpts sort;
} NodePrintOpts;

/**
 * Creates a new `Node` with a `value`.
 *
 * Returns `NULL` when:
 * - `malloc` fails to allocate a new `Node`.
 */
Node *node_new(void *value);

/**
 * Creates a new `Node` with a `parent` and a `value`.
 *
 * Returns `NULL` when:
 * - `parent` evaluates to false.
 * - `node_new` fails to create `node`.
 * - `node_add_child` fails to append `node` to `parent`.
 */
Node *node_add(Node *parent, void *value);

/**
 * Appends `child` to `parent`.
 *
 * Returns `false` when:
 * - `parent` or `child` evaluate to false.
 * - `parent` points to the same reference as `child`.
 * - `realloc` for `node->nodes` fails.
 */
bool node_add_child(Node *parent, Node *child);

/**
 * Same as `node_print_deep`, but with style options.
 *
 * Nodes with `size == 0` will be printed first always.
 * `opts` changes the function's behaviour. Check docs for `NodePrintOpts`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 * - `node->value` is `NULL`.
 * - `opts.indent_step < 1`.
 */
bool node_print(Node *node, NodePrintOpts opts);

/**
 * A "deep" print of a `Node`.
 *
 * Prints the value of `node` and recurses over each child node (and their
 * child nodes too).
 *
 * Use `node_print_shallow` to print just the immediate child nodes of `node`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_print_deep(Node *node);

/**
 * A "shallow" print of a `Node`.
 *
 * Prints the value of `node` and the value of its immediate child nodes, but
 * it doesn't recurse into deeper nodes.
 *
 * Use `node_print_deep` to print deeper nodes of `node`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_print_shallow(Node *node);

/**
 * Prints the values of `parent`, `value` and `size` of `node`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_info(Node *node);

/**
 * Frees `node` and `node->nodes`.
 *
 * Keep in mind that if you free a node that has a parent, not removing that
 * index in the parent is a reference to a dead node.
 *
 * Returns `false` when:
 * - `node` or `node->nodes` evaluate to false.
 * - Recursion fails on `node_free`.
 */
bool node_free(Node *node);

/**
 * Counts the total of child nodes in `node` (doesn't count `node`).
 *
 * Returns `0` when:
 * - `node` evaluates to false.
 */
size_t node_total(Node *node);

/**
 * Returns a NodePrintOpts with the defined default values:
 *
 * By defining the implementation macros, the default values are:
 * - indent: 0
 * - indent_step: 2
 * - sort: `NONE`
 */
NodePrintOpts nodeprintopts_default();

#endif // NODE_H_

#if !defined(__NODE_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(NODE_IMPLEMENTATION))
#define __NODE_IMPLEMENTED

static int _indent = 0;
static int _indent_step = 2;
static NodeSortOpts _sort;

#ifdef STRUCTYPES_DEBUG
static void _node_err(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define THROW(ret, msg, ...) ({ _node_err(msg, ##__VA_ARGS__); return ret; })
#else
#define THROW(ret, msg, ...) ({ return ret; })
#endif

static int _comp(const void *a, const void *b) {
    const void *n1 = (_sort == ASC) ? a : b;
    const void *n2 = (_sort == ASC) ? b : a;

    return strcmp(
        *(const char **)((Node *)n1)->value,
        *(const char **)((Node *)n2)->value
    );
}

Node *node_new(void *value) {
    Node *node = malloc(sizeof(Node));
    if (!node)
        THROW(NULL, "node_new: malloc error on initialize root");

    node->value = value;
    node->parent = NULL;
    node->nodes = NULL;
    node->size = 0;

    return node;
}

Node *node_add(Node *parent, void *value) {
    if (!parent)
        THROW(NULL, "node_add: parent evaluates to false");

    Node *node = node_new(value);
    if (!node)
        THROW(NULL, "node_add: malloc error on initialize root");

    if (!node_add_child(parent, node))
        THROW(NULL, "node_add: error adding node to parent");

    return node;
}

bool node_add_child(Node *parent, Node *child) {
    if (!parent)
        THROW(false, "node_add_child: parent evaluates to false");

    if (!child)
        THROW(false, "node_add_child: child evaluates to false");
        
    if (parent == child)
        THROW(false, "node_add_child: parent points to the same reference as child");

    void *tmp = realloc(parent->nodes, (parent->size + 1) * sizeof(Node *));
    if (!tmp)
        THROW(false, "node_add_child: realloc error");

    child->parent = parent;

    parent->nodes = tmp;
    parent->nodes[parent->size++] = child;

    return true;
}

bool node_print(Node *node, NodePrintOpts opts) {
    if (!node)
        THROW(false, "node_print: node evaluates to false");

    if (opts.indent_step < 1)
        THROW(false, "node_print: indent_step must be greater than 0");

    printf("%*s%s\n", opts.indent, "", (char *)node->value);
    if (!node->size) return true;

    size_t sizeofNode = sizeof(Node *);

    Node **zero = malloc(sizeofNode * node->size);
    Node **nonZero = malloc(sizeofNode * node->size);

    if (!zero || !nonZero) {
        free(zero);
        free(nonZero);
        THROW(false, "node_print: malloc");
    }

    size_t zeroLen = 0;
    size_t nonZeroLen = 0;

    for (size_t i = 0; i < node->size; i++) {
        if (!node->nodes[i]->size) zero[zeroLen++] = node->nodes[i];
        else nonZero[nonZeroLen++] = node->nodes[i];
    }

    if (opts.sort != NONE) {
        _sort = opts.sort;
        qsort(zero, zeroLen, sizeofNode, _comp);
        qsort(nonZero, nonZeroLen, sizeofNode, _comp);
    }

    opts.indent += opts.indent_step;

    for (size_t i = 0; i < zeroLen; i++)
        printf("%*s%s\n", opts.indent, "", (char *)zero[i]->value);

    for (size_t i = 0; i < nonZeroLen; i++)
        if (!node_print(nonZero[i], opts)) {
            free(zero);
            free(nonZero);
            THROW(false, "node_print: recursion error");
        }

    free(zero);
    free(nonZero);

    return true;
}

bool node_print_deep(Node *node) {
    if (!node) THROW(false, "node_print_deep: node evaluates to false");

    printf("%*s%s\n", _indent, "", (char *)node->value);

    _indent += _indent_step;
    size_t local_indent = _indent;

    for (size_t i = 0; i < node->size; i++) {
        Node *child = node->nodes[i];

        if (child->size > 0) {
            if (!node_print_deep(child))
                THROW(false, "node_print_deep: recursion error");
            _indent = local_indent;
        } else {
            printf("%*s%s\n", _indent, "", (char *)child->value);
        }
    }

    return true;
}

bool node_print_shallow(Node *node) {
    if (!node)
        THROW(false, "node_print_shallow: node evaluates to false");

    printf("%s\n", (char *)node->value);

    for (size_t i = 0; i < node->size; i++)
        printf("%*s%s\n", _indent_step, "", (char *)node->nodes[i]->value);

    return true;
}

bool node_info(Node *node) {
    if (!node) THROW(false, "node_info: node evaluates to false");

    printf("parent: %s\n",  node->parent
        ? (char *)node->parent->value
        : "null"
    );

    printf("value:  %s\n",  (char *)node->value);
    printf("size:   %zu\n", node->size);

    return true;
}

size_t node_total(Node *node) {
    if (!node)
        THROW(0, "node_total: node evaluates to false");

    size_t total = node->size;

    for (size_t i = 0; i < node->size; i++)
        total += node_total(node->nodes[i]);

    return total;
}

// TODO it is assumed that value is a char* on the heap to use free
bool node_free(Node *node) {
    if (!node)
        THROW(false, "node_free: node evaluates to false");

    if (node->size && !node->nodes)
        THROW(false, "node_free: node->nodes evaluates to false");

    size_t size = node->size;

    for (size_t i = size; i > 0; i--) {
        if (node->nodes[i - 1]->size > 0) {
            free(node->nodes[i - 1]->value);
            free(node->nodes[i - 1]);
            node->size--;
            continue;
        }

        if (!node_free(node->nodes[i - 1]))
            THROW(false, "node_free: recursion error");

        node->size--;
    }

    free(node->value);
    free(node->nodes);
    free(node);

    return true;
}

NodePrintOpts nodeprintopts_default() {
    return (NodePrintOpts) {
        .indent      = 0,
        .indent_step = 2,
        .sort        = ASC,
    };
}

#endif // NODE_IMPLEMENTATION
