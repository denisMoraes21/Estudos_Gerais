#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node* insert_begin(Node *head, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return head;
    }

    new_node->value = value;
    new_node->next = head;

    return new_node;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }

    printf("NULL\n");
}

void free_list(Node *head) {
    Node *current = head;

    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    Node *head = NULL;

    head = insert_begin(head, 30);
    head = insert_begin(head, 20);
    head = insert_begin(head, 10);

    print_list(head);

    free_list(head);
    return 0;
}