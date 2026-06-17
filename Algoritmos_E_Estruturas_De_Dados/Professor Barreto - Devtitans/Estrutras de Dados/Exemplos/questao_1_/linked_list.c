#include <stdio.h>
#include <stdlib.h>

// (head) value 13 : address h -> x <- List->first
// (node) value 36 : address x -> y
// (node) value 39 : address y -> z
// (node) value 52 : address z -> t
// (tail) value 65 : address t -> NULL

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct LinkedList {
    int size;
    Node *first;
} List;

List *createList(List *p_list) {
    p_list = (List *) malloc(sizeof(List));
    p_list->size = 0;
    p_list->first = NULL;
    return p_list; 
}

int emptyList(List *p_list) {
    return p_list->first == NULL;
}

void insertBegin(List *p_list, int value) {
    Node *p_new_node = (Node *) malloc(sizeof(Node));
    p_new_node->value = value;          // p_new_node : value [13] : address a -> NULL
    p_new_node->next = p_list->first;   // p_new_node : value [13] : address a -> h (value [36] : address h)
    p_list->first = p_new_node;         // p_list->first : value [13] : address h -> x (value [36] : address x)
    p_list->size = p_list->size + 1;
}

void printListPre(List *p_list) {
    /* @brief prefixed print list */
    Node *current = p_list->first;
    printf("Lista: ");

    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }

    printf("NULL\n");
}

void freeList(List *p_list) {
    Node *current = p_list->first;

    while (current != NULL) {
        Node *temp = current->next;
        free(current);
        current = temp;
    }
    free(p_list);
}

List *removeItem(List *p_list, int value) {
    Node *p_current_node = p_list->first;
    Node *p_last_node = NULL;
    while (p_current_node != NULL && p_current_node->value != value) {
        p_last_node = p_current_node;
        p_current_node = p_current_node->next;
    }
    if (p_current_node == NULL) return p_list;
    if (p_current_node == p_list->first) p_list->first = p_list->first->next;
    else p_last_node->next = p_current_node->next;
    free(p_current_node);
    printf("Valor %d removido da list\n", value);
    p_list->size = p_list->size - 1;
    return p_list;
}

Node *search(List *p_list, int value) {
    Node *p_current_node = p_list->first;
    while (p_current_node != NULL) {
        if (p_current_node->value == value) return p_current_node;
        p_current_node = p_current_node->next;
    }
    return NULL;
}

// int main() {
//     List *linked_list;
//     linked_list = createList(linked_list);
//     insertBegin(linked_list, 65);
//     insertBegin(linked_list, 52);
//     removeItem(linked_list, 65);
//     insertBegin(linked_list, 39);
//     insertBegin(linked_list, 36);
//     insertBegin(linked_list, 13);
//     printListPre(linked_list);

//     Node *node;
//     int value = 39;
//     node = search(linked_list, value);
//     if (node != NULL) printf("Valor %d encontrado\n", value); 
//     else printf("Valor %d nao encontrado\n", value);
    
//     freeList(linked_list);
//     return 0;
// }