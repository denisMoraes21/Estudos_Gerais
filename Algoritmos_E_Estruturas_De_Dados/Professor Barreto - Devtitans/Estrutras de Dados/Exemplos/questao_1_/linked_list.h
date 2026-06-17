#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct LinkedList {
    int size;
    Node *first;
} List;

List *createList(List *p_list);
int emptyList(List *p_list);
void insertBegin(List *p_list, int value);
void printListPre(List *p_list);
void freeList(List *p_list);
List *removeItem(List *p_list, int value);
Node *search(List *p_list, int value);

#endif