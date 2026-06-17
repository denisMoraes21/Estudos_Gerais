/*
 * @file    questao_5
 * @brief   Search elements
 *
 * Baixe o código arvbinbusca.c. Implementar uma função que imprima a árvore in-fixada.
 * Implementar uma função main que insira os seguintes elementos 1, 2, 3, 5, 15 e 7. Após cada
 * inserção imprima a árvore. Buscar os elementos 2 e 8, imprimindo na tela se cada elemento foi
 * encontrado ou não.
 *
 * @author Denis Moraes Guimarães
 * @date 2026-06-17
*/

#include <stdio.h>
#include "trees.h"

void printTree(s_binary_tree *branch) {
    printf("Pré-fixado: ");
    printPre(branch);
    printf("\n");

    printf("In-fixado: ");
    printBSequence(branch);
    printf("\n");

    printf("Pós-fixado: ");
    printPos(branch);
    printf("\n");
}

void printFound(s_binary_tree *branch, int number) {
    if (search(branch, number)) printf("Number %d found!\n", number);
    else printf("Number %d not found!\n", number);
}

int main(void) {

    // Insert new branch on root
    s_binary_tree *branch_1 = create(1, NULL, NULL);
    printTree(branch_1);

    s_binary_tree *branch_2 = create(2, branch_1, NULL);
    printTree(branch_2);

    s_binary_tree *branch_3 = create(3, branch_2, NULL);
    printTree(branch_3);
    
    s_binary_tree *branch_5 = create(5, branch_3, NULL);
    printTree(branch_5);    

    s_binary_tree *branch_7 = create(7, NULL, NULL);
    printTree(branch_7);
    
    s_binary_tree *branch_15 = create(15, branch_5, branch_7);
    printTree(branch_15);

    // Check number on tree
    printFound(branch_15, 2);
    printFound(branch_15, 8);

    return 0;
}