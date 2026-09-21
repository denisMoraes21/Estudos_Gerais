#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <stddef.h>

#define MAX_NODES 3

struct s_list {
    int v_data;
    struct s_list *p_next;
};

typedef struct s_list s_nodeList;
typedef s_nodeList *p_nodeList;

void f_insert_in_last(p_nodeList *p_begin, char v_newData) {
    p_nodeList p_newNode = malloc(sizeof(s_nodeList));

    const bool v_is_newNode_not_null = p_newNode != NULL;
    if (v_is_newNode_not_null) {

        p_newNode->v_data = v_newData;
        p_newNode->p_next = NULL;

        p_nodeList p_previousNode = NULL;
        p_nodeList p_currentNode = *p_begin;
        
        while (1){

            bool v_is_currentNode_null = p_currentNode == NULL;

            if (v_is_currentNode_null) break;    

            bool v_is_newData_less_than_currentData = v_newData <= p_currentNode->v_data;
            
            if (v_is_newData_less_than_currentData) break;    
        
            p_previousNode = p_currentNode;
            p_currentNode = p_currentNode->p_next;
        }
        
        bool v_is_previousNode_null = p_previousNode == NULL;
        if (v_is_previousNode_null) {
            p_newNode->p_next = *p_begin;
            *p_begin = p_newNode;
        }
        else {
            p_previousNode->p_next = p_newNode;
            p_newNode->p_next = p_currentNode;
        }
    } 
    else {
        printf("%c not inserted. No memory available.\n", v_newData);
    }
}

void f_insert_in_begin(p_nodeList *p_begin, char v_newData, size_t *p_count){
    p_nodeList p_newNode = malloc(sizeof(s_nodeList));

    const bool v_is_newNode_null = p_newNode == NULL;
    if (v_is_newNode_null) {
        printf("%c not inserted. No memory available.\n", v_newData);
        return;
    }

    const bool v_is_counting_grather_than_max = *p_count >= MAX_NODES;
    if (v_is_counting_grather_than_max) {
        printf("List full. Not possible insert: %c", v_newData);
        return;
    }

    p_newNode->v_data = v_newData;
    p_newNode->p_next = *p_begin;
    *p_begin = p_newNode;

    (*p_count)++;
}

void f_delete_by_node(p_nodeList *p_begin, int v_position);

void f_delete_by_value(p_nodeList *p_begin, char value);

int main(void) {
    p_nodeList p_beginNodeList_1 = NULL; 
    p_nodeList p_beginNodeList_2 = NULL; 
    char v_item = '\0';
    size_t v_count = 0;

    v_item = 'A';
    f_insert_in_begin(&p_beginNodeList_1, v_item, &v_count);
    f_insert_in_last(&p_beginNodeList_2, v_item);

    v_item = 'B';
    f_insert_in_begin(&p_beginNodeList_1, v_item, &v_count);
    f_insert_in_last(&p_beginNodeList_2, v_item);

    v_item = 'C';
    f_insert_in_begin(&p_beginNodeList_1, v_item, &v_count);
    f_insert_in_last(&p_beginNodeList_2, v_item);

    v_item = 'D';
    f_insert_in_begin(&p_beginNodeList_2, v_item, &v_count);
    f_insert_in_last(&p_beginNodeList_2, v_item);

    return 0;
}