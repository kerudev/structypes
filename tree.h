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
 * Returns `false` when:
 * - `realloc` for `node->nodes` fails.
 * - `node_new` fails.
 */
bool node_new_child(Node *node, void *value);

/**
 * Returns `false` when:
 * - `realloc` for `node->nodes` fails.
 */
bool node_add_child(Node *node, Node *child);

/**
 * A "deep" print of a `Node`.
 *
 * Prints the value of `tree` and recurses over each child node (and their
 * child nodes too).
 * 
 * Use `node_print` to print just the immediate child nodes of `tree`.
 *
 * Returns `false` when:
 * - `tree` evaluates to false.
 */
bool tree_print(Tree *tree);

/**
 * A "shallow" print of a `Node`.
 *
 * Prints the value of `node` and the value of its immediate child nodes, but
 * it doesn't recurse into deeper nodes.
 *
 * Use `tree_print` to print deeper nodes of `node`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_print(Node *node);

/**
 * Prints the values of `parent`, `value` and `size` of `node`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_info(Node *node);

/**
 * Frees `tree` and `tree->nodes`.
 *
 * Returns `false` when:
 * - `tree` or `tree->nodes` evaluate to false.
 */
bool tree_free(Tree *tree);

/**
 * Frees `node` and child nodes if `node->size > 0`.
 *
 * Returns `false` when:
 * - `node` evaluates to false.
 */
bool node_free(Node *node);

#endif // TREE_H_

#if !defined(__TREE_IMPLEMENTED) && (defined(STRUCTYPES_IMPLEMENTATION) || defined(TREE_IMPLEMENTATION))
#define __TREE_IMPLEMENTED

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

bool tree_print(Tree *tree) {
    if (!tree) return false;

    printf("%s\n", tree->value);

    for (size_t i = 0; i < tree->size; i++) {
        Node *node = tree->nodes[i];

        if (node->size > 0) {
            tree_print(node);
        } else {
            printf("  %s\n", node->value);
        }
    }

    return true;
}

bool node_print(Node *node) {
    if (!node) return false;

    printf("%s\n", node->value);

    for (size_t i = 0; i < node->size; i++)
        printf("  %s\n", node->nodes[i]->value);

    return true;
}

bool node_info(Node *node) {
    if (!node) return false;

    printf("parent: %s\n",  node->parent ? node->parent->value : "null");
    printf("value:  %s\n",  node->value);
    printf("size:   %zu\n", node->size);

    return true;
}

bool tree_free(Tree *tree) {
    if (!tree || !tree->nodes) return false;

    for (size_t i = 0; i < tree->size; i++) {
        if (tree->nodes[i]->size > 0) {
            free(tree->nodes[i]);
            continue;
        }

        tree_free(tree->nodes[i]);
    }

    free(tree->nodes);
    free(tree);

    return true;
}

// TODO not removing node in parent->nodes might cause a reference to a dead node
bool node_free(Node *node) {
    if (!node) return false;

    if (node->size > 0)
        tree_free(node);

    free(node);

    return true;
}

#endif // TREE_IMPLEMENTATION
