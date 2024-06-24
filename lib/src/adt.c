#include "../include/adt.h"

LIST * list_add(LIST *list, void *data)
{
    LIST * new_node = k_malloc(sizeof(LIST));
    new_node -> data = data;
    new_node -> next = NULL;
    if(list == NULL)
    {
        list = new_node;
        printf("after malloc list: %x\n\r", list);
    }
    else
    {
        LIST * temp = list;
        while(temp -> next != NULL)
        {
            temp = temp -> next;
        }
        temp -> next = new_node;
    }

    return list;
}