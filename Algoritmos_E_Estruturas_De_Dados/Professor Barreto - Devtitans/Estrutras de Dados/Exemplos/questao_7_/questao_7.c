/*
 * @file    questao_7
 * @brief   Compare order
 *
 * Gere um programa em C que: (i) gere exatamente a árvore genérica abaixo; (ii) implemente
 * uma função de busca nesta subárvore, por exemplo, que busque o nó que contém o “J”; e (iii)
 * implemente uma função que remova da árvore a subárvore cuja raiz possui determinado caractere
 * (por exemplo, a subárvore que tem como raiz o nó “G”) e liberar a respectiva memória; e (iv) liberar
 * toda a memória árvore genérica resultante.
 *
 * @author Denis Moraes Guimarães
 * @date 2026-06-17
*/

#include <stdio.h>
#include "trees.h"


int main(void) {

    // Insert new branch on root
    s_generic_tree *a = createGTree('A');
    s_generic_tree *b = createGTree('B');
    s_generic_tree *c = createGTree('C');
    s_generic_tree *d = createGTree('D');
    s_generic_tree *e = createGTree('E');
    s_generic_tree *f = createGTree('F');
    s_generic_tree *g = createGTree('G');
    s_generic_tree *h = createGTree('H');
    s_generic_tree *i = createGTree('I');
    s_generic_tree *j = createGTree('J');

    // C
    insertTree(c, d);

    // B
    insertTree(b, e);
    insertTree(b, c);
    
    // G
    insertTree(g, h);
    insertTree(g, i);

    // I
    insertTree(i, j);

    // A
    insertTree(a, g);
    insertTree(a, f);
    insertTree(a, b);

    char letter = 'J';
    if (searchTree(a, letter)) {
        printf("Letter %c found!\n", letter);
    }
    else {
        printf("Letter %c not found!\n", letter);
    }

    f->next = NULL;   // poda a subárvore G

    freeTree(g);

    printTree(a);

    freeTree(a);
    
    return 0;
}