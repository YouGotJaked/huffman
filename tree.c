/* 
 * File:  tree.c
 * Author: Jake Day
 * Class: COEN 12
 * Instructor: Atkinson
 * Section: T 5:15-8pm
 * Created on May 17, 2016, 5:38 PM
 * Description: This file implements a binary tree abstract data type.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

void cut(struct tree *child);

struct tree {
    int data;
    struct tree *left, *right, *parent;
};

/*
 * Big-O Notation: O(1)
 * Allocates memory for a new binary tree and returns a tree pointer
 */
struct tree *createTree(int data, struct tree *left, struct tree *right) {
    struct tree *root;

    root = malloc(sizeof (struct tree));
    assert(root != NULL);

    root->data = data;
    root->left = left;
    root->right = right;
    root->parent = NULL;

    if (left != NULL) {
        cut(left);
        left->parent = root;
    }

    if (right != NULL) {
        cut(right);
        right->parent = root;
    }

    return root;
}

/*
 * Big-O Notation: O(n)
 * Frees all subtree pointers under a given root
 */
void destroyTree(struct tree *root) {
    if (root != NULL) {
        destroyTree(root->left);
        destroyTree(root->right);
        free(root);
    }
}

/*
 * Big-O Notation: O(1)
 * Returns the root's data
 */
int getData(struct tree *root) {
    assert(root != NULL);
    return root->data;
}

/*
 * Big-O Notation: O(1)
 * Returns the left child under a given root
 */
struct tree *getLeft(struct tree *root) {
    assert(root != NULL);
    return root->left;
}

/*
 * Big-O Notation: O(1)
 * Returns the right child under a given root
 */
struct tree *getRight(struct tree *root) {
    assert(root != NULL);
    return root->right;
}

/*
 * Big-O Notation: O(1)
 * Returns the parent child of a given root
 */
struct tree *getParent(struct tree *root) {
    assert(root != NULL);
    return root->parent;
}

/*
 * Big-O Notation: O(1)
 * Updates the left child of a root with a new left tree
 */
void setLeft(struct tree *root, struct tree *left) {
    assert(root != NULL);

    if (root->left != NULL) {
        root->left->parent = NULL;
    }

    root->left = left;

    cut(left);

    left->parent = root;
}

/*
 * Big-O Notation: O(1)
 * Updates the right child of a root with a new right tree
 */
void setRight(struct tree *root, struct tree *right) {
    assert(root != NULL);

    if (root->right != NULL) {
        root->right->parent = NULL;
    }

    root->right = right;

    cut(right);

    right->parent = root;
}

/*
 * Big-O Notation: O(1)
 * Cuts a given tree from its parent
 */
void cut(struct tree *child) {
    if (child->parent != NULL) {
        if (child->parent->left == child) {
            child->parent->left = NULL;
        }
        if (child->parent->right == child) {
            child->parent->right = NULL;
        }
    }
}