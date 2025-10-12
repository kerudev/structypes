// tree.h - A tree implementation.
//
// Useful macros:
// - STRUCTYPES_IMPLEMENTATION: defines all of structype's implementations.
// - TREE_IMPLEMENTATION: implementations of tree functions.

#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** A node. */
typedef struct Node {
    /** Value of the node. */
    void        *value;
    /** Pointer to the parent node. */
    struct Node *parent;
    /** Array of children nodes. */
    struct Node **nodes;
    /** Number of child nodes. */
    size_t      size;
} Node;

/**
 * A node tree.
 *
 * The only difference with `Node` is that `Tree` instances should not have a
 * `parent` node. When a `parent` is defined, the `Tree` instance should be
 * treated as a `Node` instance.
 */
typedef Node Tree;

typedef enum {
    NONE,
    ASC,
    DESC,
} NodeSortOpts;

typedef struct {
    unsigned int indent;
    unsigned int indent_step;
    NodeSortOpts sort;
} NodePrintOpts;

// TODO error handling messages

/**
 * Creates a new `Tree` with a `value`.
 * See the documentation of `Tree` to know its differences with `Node`.
 *
 * Returns `NULL` when:
 * - `malloc` fails to allocate a new `Tree`.
 */
Tree *tree_new(void *value);

/**
 * Creates a new `Node` with a `parent` and a `value`.
 * See the documentation of `Tree` to know its differences with `Node`.
 *
 * Returns `NULL` when:
 * - `malloc` fails to allocate a new `Node`.
 */
Node *node_new(Node *parent, void *value);

/**
 * Creates a node with `value` inside of `node`. 
 *
 * Returns `false` when:
 * - `realloc` for `node->nodes` fails.
 * - `node_new` fails.
 */
bool node_new_child(Node *node, void *value);

/**
 * Appends `child` to `parent`.
 *
 * Returns `false` when:
 * - `realloc` for `node->nodes` fails.
 */
bool node_add_child(Node *parent, Node *child);

/**
 * Same as `node_print_deep`, but with style options.
 *
 * Nodes with `size == 0` will be printed first always.
 *
 * `opts` changes the function's behaviour:
 * - indent: starting indentation.
 * - indent_step: how much indent will increment on each level.
 * - sort: how nodes will be ordered by their value.
 *   - `NONE`: insertion order (no sort).
 *   - `ASC` : ascending order.
 *   - `DESC`: descending order.
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
bool node_print_deep(Node *tree);

/**
 * A "shallow" print of a `Node`.
 *
 * Prints the value of `node` and the value of its immediate child nodes, but
 * it doesn't recurse into deeper nodes.
 *
 * Use `tree_print_deep` to print deeper nodes of `node`.
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
 * Returns a NodePrintOpts with the defined default values:
 * 
 * By defining the implementation macros, the default values are:
 * - indent: 0
 * - indent_step: 2
 * - sort: `NONE`
 */
NodePrintOpts nodeprintopts_default();

#endif // TREE_H_

#if !defined(__TREE_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(TREE_IMPLEMENTATION))
#define __TREE_IMPLEMENTED

static int _indent = 0;
static int _indent_step = 2;
static NodeSortOpts _sort;

static int _comp(const void *a, const void *b) {
    const void *n1 = (_sort == ASC) ? a : b;
    const void *n2 = (_sort == ASC) ? b : a;

    return strcmp(
        *(const char **)((Node *)n1)->value,
        *(const char **)((Node *)n2)->value
    );
}

Tree *tree_new(void *value) {
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    if (!tree) {
        printf("tree_new: malloc error on initialize root");
        return NULL;
    }

    tree->value = value;
    tree->parent = NULL;
    tree->nodes = NULL;
    tree->size = 0;

    return tree;
}

Node *node_new(Node *parent, void *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        printf("node_new: malloc error on initialize root");
        return NULL;
    }

    node->value = value;
    node->parent = parent;
    node->nodes = NULL;
    node->size = 0;

    return node;
}

bool node_new_child(Node *node, void *value) {
    void *tmp = realloc(node->nodes, (node->size + 1) * sizeof(Node *));
    if (!tmp) {
        printf("node_new_child: realloc error");
        return false;
    }

    node->nodes = (Node **)tmp;

    Node *newNode = node_new(node, value);
    if (!newNode) {
        printf("node_new_child: error creating new node");
        return false;
    }

    node->nodes[node->size] = newNode;
    node->size++;

    return true;
}

bool node_add_child(Node *parent, Node *child) {
    void *tmp = realloc(parent->nodes, (parent->size + 1) * sizeof(Node *));
    if (!tmp) {
        printf("node_add_child: realloc error");
        return false;
    }

    parent->nodes = (Node **)tmp;
    parent->nodes[parent->size] = child;
    parent->size++;

    return true;
}

bool node_print(Node *node, NodePrintOpts opts) {
    if (!node || node->value == NULL || opts.indent_step < 1) return false;

    printf("%*s%s\n", opts.indent, "", node->value);
    if (!node->size) return true;

    size_t sizeofNode = sizeof(Node *);

    Node **zero = (Node **) malloc(sizeofNode * node->size);
    Node **nonZero = (Node **) malloc(sizeofNode * node->size);

    if (!zero || !nonZero) {
        free(zero);
        free(nonZero);
        return false;
    }

    int zeroLen = 0;
    int nonZeroLen = 0;

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
        printf("%*s%s\n", opts.indent, "", zero[i]->value);

    for (size_t i = 0; i < nonZeroLen; i++)
        if (!node_print(nonZero[i], opts)) {
            free(zero);
            free(nonZero);
            return false;
        }

    free(zero);
    free(nonZero);

    return true;
}

bool node_print_deep(Node *node) {
    if (!node) return false;

    printf("%*s%s\n", _indent, "", node->value);

    _indent += _indent_step;
    size_t local_indent = _indent;

    for (size_t i = 0; i < node->size; i++) {
        Node *node = node->nodes[i];

        if (node->size > 0) {
            if (!node_print_deep(node)) return false;
            _indent = local_indent;
        } else {
            printf("%*s%s\n", _indent, "", node->value);
        }
    }

    return true;
}

bool node_print_shallow(Node *node) {
    if (!node) return false;

    printf("%s\n", node->value);

    for (size_t i = 0; i < node->size; i++)
        printf("%*s%s\n", _indent_step, "", node->nodes[i]->value);

    return true;
}

bool node_info(Node *node) {
    if (!node) return false;

    printf("parent: %s\n",  node->parent ? node->parent->value : "null");
    printf("value:  %s\n",  node->value);
    printf("size:   %zu\n", node->size);

    return true;
}

bool node_free(Node *node) {
    if (!node || !node->nodes) return false;

    for (size_t i = 0; i < node->size; i++) {
        if (node->nodes[i]->size > 0) {
            free(node->nodes[i]);
            continue;
        }

        if (!node_free(node->nodes[i])) return false;
    }

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

#endif // TREE_IMPLEMENTATION
