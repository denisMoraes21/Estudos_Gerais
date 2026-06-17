/*
 * @file    questao_1
 * @brief   compare linked lists
 *
 * Baixe o arquivo listas.c e implemente a função "igual", que compara duas listas encadeadas e
 * retorna 1 se forem iguais e 0 caso contrário
 * 
 * @author Denis Moraes Guimarães
 * @date 2026-06-17
*/
#include <stdio.h>
#include "linked_list.h"

int main(void) {

    // Declare and initialize list structs
    List *p_linked_list_1 = createList(NULL);
    List *p_linked_list_2 = createList(NULL);
    
    // Values on list
    int value_1 = 10;
    int value_2 = 20;
    int value_3 = 30;

    // Insert values on list 1
    insertBegin(p_linked_list_1, value_1);
    insertBegin(p_linked_list_1, value_2);
    insertBegin(p_linked_list_1, value_3);

    // Insert values on list 2
    insertBegin(p_linked_list_2, value_1);
    insertBegin(p_linked_list_2, value_2);
    insertBegin(p_linked_list_2, value_3);

    // Compare size lists
    if (p_linked_list_1->size != p_linked_list_2->size) {
        printf("Lists don't have same size!\n");
        return 1;
    }

    // Compare items
    Node *current_node_list_1 = p_linked_list_1->first;
    Node *current_node_list_2 = p_linked_list_2->first;
    
    int equals = 1;
    while (current_node_list_1 != NULL && current_node_list_2 != NULL) {
        if (current_node_list_1->value != current_node_list_2->value) {
            equals = 0;
            break;
        }

        current_node_list_1 = current_node_list_1->next;
        current_node_list_2 = current_node_list_2->next;
    }
    
    // Clean memory lists
    freeList(p_linked_list_1);
    freeList(p_linked_list_2);

    if (equals) {
        printf("Lists are equal!\n"); 
        return 0;
    } 
    printf("Lists are not equal!");
    return 1;
}