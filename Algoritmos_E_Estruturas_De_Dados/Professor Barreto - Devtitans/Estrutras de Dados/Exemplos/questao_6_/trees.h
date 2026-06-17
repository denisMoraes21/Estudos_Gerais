#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

/* Binary Tree */

typedef struct binary_tree {
    int info;
    struct binary_tree* p_branch_left;
    struct binary_tree* p_branch_right;
} s_binary_tree;

/* Generic Binary Search Tree */

typedef struct generic_tree {
    int info;
    struct generic_tree* p_branch_left;
    struct generic_tree* p_branch_right;
} s_generic_tree;

/* Binary Tree Functions */

s_binary_tree *init(void);

bool emptyBTree(s_binary_tree *p_tree);

s_binary_tree* create(
    char letter,
    s_binary_tree* p_branch_left,
    s_binary_tree* p_branch_right
);

void printPre(s_binary_tree* p_tree);

void printBSequence(s_binary_tree* p_tree);

void printPos(s_binary_tree* p_tree);

s_binary_tree* freeBTree(s_binary_tree* p_tree);

int search(s_binary_tree* p_tree, char letter);

/* Generic Tree Functions */

bool emptyGTree(s_generic_tree* p_tree);

void printGTSequence(s_generic_tree* p_tree);

s_generic_tree* searchGT(
    s_generic_tree* p_tree,
    int value
);

s_generic_tree* insertGT(
    s_generic_tree* p_tree,
    int value
);

s_generic_tree* freeGT(
    s_generic_tree* p_tree
);

#endif /* TREE_H */