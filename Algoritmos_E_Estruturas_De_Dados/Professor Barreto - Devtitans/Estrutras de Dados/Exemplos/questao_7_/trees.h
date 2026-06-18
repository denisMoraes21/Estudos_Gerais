#ifndef TREE_H
#define TREE_H

typedef struct generic_tree {
    char value;
    struct generic_tree* first;
    struct generic_tree* next;
} s_generic_tree;

s_generic_tree* createGTree(char value);

void insertTree(s_generic_tree* p_tree_a, s_generic_tree* p_tree_f);

void freeTree(s_generic_tree* p_tree_a);

void printTree(s_generic_tree* p_tree_a);

int searchTree(s_generic_tree* p_tree_a, char letter);

#endif /* TREE_H */