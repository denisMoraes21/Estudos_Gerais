#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct binary_tree {
    int info;
    struct binary_tree* p_branch_left;
    struct binary_tree* p_branch_right;
} s_binary_tree;

typedef struct generic_tree {
    int info;
    struct generic_tree* p_branch_left;
    struct generic_tree* p_branch_right;
} s_generic_tree;

s_binary_tree *init (void) {
    return NULL;
}

bool emptyBTree(s_binary_tree *p_tree) {
    return p_tree == NULL;
}

bool emptyGTree(s_generic_tree *p_tree) {
    return p_tree == NULL;
}

s_binary_tree* create(int number, s_binary_tree* p_branch_left, s_binary_tree *p_branch_right) {
    s_binary_tree* p_tree = (s_binary_tree *)malloc(sizeof(s_binary_tree));
    p_tree->info = number;
    p_tree->p_branch_left = p_branch_left;
    p_tree->p_branch_right = p_branch_right;
    return p_tree;
}

void printPre(s_binary_tree* p_tree) {
    if (!emptyBTree(p_tree)) {
        printf("%d ", p_tree->info);
        printPre(p_tree->p_branch_left);
        printPre(p_tree->p_branch_right);
    }
}

void printBSequence(s_binary_tree *p_tree) {
    if (!emptyBTree(p_tree)) {
        printBSequence(p_tree->p_branch_left);
        printf("%d ", p_tree->info);
        printBSequence(p_tree->p_branch_right);
    }
}

void printGTSequence(s_generic_tree *p_tree) {
    if (!emptyGTree(p_tree)) {
        printGTSequence(p_tree->p_branch_left);
        printf("%d ", p_tree->info);
        printGTSequence(p_tree->p_branch_right);
    }
}

void printPos(s_binary_tree* p_tree) {
    if (!emptyBTree(p_tree)) {
        printPos(p_tree->p_branch_left);
        printPos(p_tree->p_branch_right);
        printf("%d ", p_tree->info);
    }
}

s_binary_tree* freeBTree(s_binary_tree *p_tree) {
    if (!emptyBTree(p_tree)) {
        freeBTree(p_tree->p_branch_left);
        freeBTree(p_tree->p_branch_right);
        free(p_tree);
    }
    return NULL;
}

int search(s_binary_tree *p_tree, char letter) {
    if (emptyBTree(p_tree)) {
        return 0;
    }
    else {
        return p_tree->info==letter || search(p_tree->p_branch_left, letter) || search(p_tree->p_branch_right, letter);
    }
}

s_generic_tree* searchGT(s_generic_tree* p_tree, int value) {
    if (p_tree == NULL) return NULL;
    else if (p_tree->info > value) return searchGT(p_tree->p_branch_left, value);
    else if (p_tree->info < value) return searchGT(p_tree->p_branch_right, value);
    else return p_tree;
}

s_generic_tree* insertGT(s_generic_tree* p_tree, int value) {
    if (p_tree == NULL) {
        p_tree = (s_generic_tree *)malloc(sizeof(s_generic_tree));
        p_tree->info = value;
        p_tree->p_branch_left = p_tree->p_branch_right = NULL;
    }
    else if (value < p_tree->info) {
        p_tree->p_branch_left = insertGT(p_tree->p_branch_left, value);
    }
    else if (value > p_tree->info) {
        p_tree->p_branch_right = insertGT(p_tree->p_branch_right, value);
    }
    return p_tree;
}

s_generic_tree* freeGT(s_generic_tree *p_tree) {
    if (!emptyGTree(p_tree)) {
        freeGT(p_tree->p_branch_left);
        freeGT(p_tree->p_branch_right);
        free(p_tree);
    }
    return NULL;
}

// int main() {
//     s_binary_tree* d = create('d', NULL, NULL);
//     s_binary_tree* b = create('b', init(), d);
//     s_binary_tree* e = create('e', init(), init());
//     s_binary_tree* f = create('f', init(), init());
//     s_binary_tree* c = create('c', e, f);
//     s_binary_tree* a = create('a', b,c);
    
//     printPre(a); printf("\n");
//     printBSequence(a); printf("\n");
//     printPos(a); printf("\n");
//     printBSequence(c); printf("\n");

//     char caractere;
//     caractere = 'f';
//     if (search(a, caractere)) printf("Caractere %c encontrado!\n", caractere);
//     else printf("Caractere %c nao encontrado!\n", caractere);

//     // Enxerto
//     a->p_branch_left->p_branch_left = create(
//         'x', 
//         create(
//             'y',
//             init(),
//             init()), 
//             create(
//                 'z',
//                 init(),
//                 init()
//             )
//         );
//     printBSequence(a); printf("\n");

//     // Poda
//     a->p_branch_right->p_branch_left = freeBTree(
//         a->p_branch_right->p_branch_left
//     );   // elimina o 'e'
//     printBSequence(a); printf("\n");

//     a = freeBTree(a);

//     int valor;
//     s_generic_tree* p_generic_tree = NULL;
//     p_generic_tree = insertGT(p_generic_tree, 30);
//     p_generic_tree = insertGT(p_generic_tree, 10);
//     p_generic_tree = insertGT(p_generic_tree, 58);
//     p_generic_tree = insertGT(p_generic_tree, 41);
//     p_generic_tree = insertGT(p_generic_tree, 60);
//     p_generic_tree = insertGT(p_generic_tree, 5);

//     printGTSequence(p_generic_tree); printf("\n");

//     valor = 58;
//     if (searchGT(p_generic_tree, valor)) printf("Valor %d encontrado\n", valor);
//     else printf("Valor %d nao encontrado\n", valor);

//     valor = 69;
//     if (searchGT(p_generic_tree, valor)) printf("Valor %d encontrado\n", valor);
//     else printf("Valor %d nao encontrado\n", valor);

// }