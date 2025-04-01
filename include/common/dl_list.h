#pragma once

#include <common/operation.h>

struct dl_list
{
    struct dl_list *previous;
    struct dl_list *next;
};

#define DL_LIST_INIT(list)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        (list)->previous = list;                                                                                       \
        (list)->next = list;                                                                                           \
    } while (0)

#define DL_LSIT_HEAD(list) ((list->next))

#define DL_LIST_ADD(list, node)                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        (node)->previous = list;                                                                                       \
        (node)->next = (list)->next;                                                                                   \
        (list)->next = (node);                                                                                         \
        ((list)->next)->previous = (node);                                                                             \
    } while (0)

#define DL_LIST_ADD_TAIL(list, node) DL_LIST_ADD(((list)->previous), (node))

#define DL_LIST_DELETE(node)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        ((node)->next)->previous = (node)->previous;                                                                   \
        ((node)->previous)->next = (node)->next;                                                                       \
        (node)->previous = (DL_LIST *)NULL;                                                                            \
        (node)->next = (DL_LIST *)NULL;                                                                                \
    } while (0)

#define DL_LIST_EMPTY(list) ((list)->next == list)

#define DL_LIST_CONTAINER(item, type, member) ((type *)(void *)((char *)(item)-OFFSET_OF(type, member)))
