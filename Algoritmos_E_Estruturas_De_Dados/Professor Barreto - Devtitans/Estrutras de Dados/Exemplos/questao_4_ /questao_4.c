/*
 * @file    questao_4
 * @brief   compare linked lists
 *
 * Podar a sub-árvore que possui raiz com a informação “5” (inclusive). Utilize a função de busca
 * para buscar o nó com valor “5” e, depois disso, fazer a poda. Imprimir a árvore resultante usando
 * os três métodos (pré-fixado, in-fixado e pós-fixado).
 *
 * @author Denis Moraes Guimarães
 * @date 2026-06-17
*/

#include <stdio.h>
#include "trees.h"

int main(void) {
    s_binary_tree *branch_12 = create(12, NULL, NULL);
    s_binary_tree *branch_67 = create(67, NULL, NULL);
    s_binary_tree *branch_34 = create(34, init(), branch_67);
    s_binary_tree *branch_5 = create(5, branch_12, branch_34);
    s_binary_tree *branch_40 = create(40, NULL, NULL);
    s_binary_tree *branch_80 = create(80, init(), branch_40);
    s_binary_tree *branch_45 = create(45, branch_5, branch_80);

    branch_45->p_branch_right->p_branch_left = create(
        3, 
        create(
            66,
            init(),
            init()), 
        create(
            31,
            init(),
            init())
    );

    // Poda
    branch_45->p_branch_left = freeBTree(branch_45->p_branch_left);
    
    printf("Pré-fixado: ");
    printPre(branch_45);
    printf("\n");

    printf("In-fixado: ");
    printBSequence(branch_45);
    printf("\n");

    printf("Pós-fixado: ");
    printPos(branch_45);
    printf("\n");

    if (search(branch_45, 80)) printf("Number found!\n");
    
    return 0;
}