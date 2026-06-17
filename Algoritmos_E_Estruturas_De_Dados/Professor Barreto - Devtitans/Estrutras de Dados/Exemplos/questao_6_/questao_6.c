/*
 * @file    questao_6
 * @brief   Compare order
 *
 * Baseado no código arvbinbusca.c insira os seguintes elementos: 2, 4, 6, 8, 10, 12, 14, 16 e 18.
 * Imprima em ordem infixada. Você consegue perceber algum “problema” neste uso específico da
 * árvore binária de busca com esses dados?
 *
 * @author Denis Moraes Guimarães
 * @date 2026-06-17
*/

#include <stdio.h>
#include "trees.h"

void printTree(s_binary_tree *branch) {
    printf("In-fixado: ");
    printBSequence(branch);
    printf("\n");
}

int main(void) {

    // Insert new branch on root
    s_binary_tree *branch_2 = create(2, NULL, NULL);
    printTree(branch_2);

    s_binary_tree *branch_4 = create(4, branch_2, NULL);
    printTree(branch_4);

    s_binary_tree *branch_6 = create(6, branch_4, NULL);
    printTree(branch_6);
    
    s_binary_tree *branch_8 = create(8, branch_6, NULL);
    printTree(branch_8);    

    s_binary_tree *branch_10 = create(10, branch_8, NULL);
    printTree(branch_10);

    s_binary_tree *branch_12 = create(12, branch_10, NULL);
    printTree(branch_10);
    
    s_binary_tree *branch_14 = create(14, branch_12, NULL);
    printTree(branch_14);

    s_binary_tree *branch_16 = create(16, branch_14, NULL);
    printTree(branch_16);

    s_binary_tree *branch_18 = create(18, branch_16, NULL);
    printTree(branch_18);

    // Árvore desbalanceada
    // Se comporta como uma lista encadeada
    // Normalmente a busca teria complexidade BigO O(log n), pois as buscas são cortadas pela 
    // metada a cada iteração.
    // Nesse caso a complexidade temporal ficou mais lenta, O(n);

    return 0;
}