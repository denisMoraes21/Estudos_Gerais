/*
 * @file    questao_2
 * @brief   compare linked lists
 *
 * Baixe o código arvbin.c. Implemente três funções para imprimir todos elementos da árvore
 * usando os métodos pré-fixado, in-fixado e pós-fixado. Implemente uma função main que, usando
 * as operações inicializa e cria, gere a seguinte árvore e depois imprima usando os três métodos.
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
    
    printf("Pré-fixado: ");
    printPre(branch_45);
    printf("\n");

    printf("In-fixado: ");
    printBSequence(branch_45);
    printf("\n");

    printf("Pós-fixado: ");
    printPos(branch_45);
    printf("\n");
    
    return 0;
}