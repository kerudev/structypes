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

typedef struct Node {
    struct Node *parent;
    struct Node **nodes;
    void        *value;
    size_t      size;
} Node;

typedef Node Tree;

// TODO documentation
// TODO error handling messages

Tree *tree_new(void *value);
Node *node_new(Tree *parent, void *value);

bool node_new_child(Node *node, void *value);
bool node_add_child(Node *node, Node *child);

bool tree_print(Tree *tree);
bool node_print(Node *node);
bool node_info(Node *node);

bool tree_free(Tree *tree);
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

    tree->parent = NULL;
    tree->nodes = NULL;
    tree->value = value;
    tree->size = 0;

    return tree;
}

Node *node_new(Tree *parent, void *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        printf("node_new: malloc error on initialize root");
        return NULL;
    }

    node->parent = parent;
    node->nodes = NULL;
    node->value = value;
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
