#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


s_generic_tree* createGTree(char value) {
    s_generic_tree *p_tree = (s_generic_tree *)malloc(sizeof(s_generic_tree));
    p_tree->value = value;
    p_tree->first = NULL;
    p_tree->next = NULL;
    return p_tree;
}

void insertTree(s_generic_tree* p_tree_a, s_generic_tree* p_tree_f) {
    p_tree_f->next = p_tree_a->first;
    p_tree_a->first = p_tree_f;
}

void freeTree(s_generic_tree* p_tree_a) {
    s_generic_tree* p_tree = p_tree_a->first;

    while (p_tree != NULL) {
        s_generic_tree* p_tree_t = p_tree->next;
        freeTree(p_tree);
        p_tree = p_tree_t;
    }

    free(p_tree_a);
} 

void printTree(s_generic_tree* p_tree_a) {
    s_generic_tree *p_tree;
    printf("%c\n", p_tree_a->value);
    for (p_tree = p_tree_a->first; p_tree != NULL; p_tree=p_tree->next) {
        printTree(p_tree);
    }
}

int searchTree(s_generic_tree* p_tree_a, char letter) {
    s_generic_tree* p_tree;
    if (p_tree_a->value == letter) {
        return 1;
    }
    else {
        for (p_tree = p_tree_a->first; p_tree != NULL; p_tree = p_tree->next) {
            if (searchTree(p_tree, letter)) return 1;
        }
    }
    return 0;
}
