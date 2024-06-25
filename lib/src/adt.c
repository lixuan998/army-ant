#include "../include/adt.h"

LOOP_LIST *list_add(LOOP_LIST *list, void *data)
{
    LOOP_LIST *new_node = k_malloc(sizeof(LOOP_LIST));
    new_node->data = data;
    new_node->next = new_node;
    if (list == NULL)
    {
        list = new_node;
        printf("after malloc list: %x\n\r", list);
    }
    else
    {
        LOOP_LIST *temp = list;
        while (temp->next != list)
        {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }

    return list;
}