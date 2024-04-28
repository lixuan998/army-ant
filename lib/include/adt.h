#ifndef __ADT_H__
#define __ADT_H__

#include "arch/defs.h"

typedef struct _PRIORITY_QUEUE{
    void *data;
    struct _PRIORITY_QUEUE *left;
    struct _PRIORITY_QUEUE *right;
} PRIORITY_QUEUE;

typedef struct _LIST{
    void *data;
    struct _LIST *next;
} LIST;

LIST * list_add(LIST * list, void * data);

#endif /* __ADT_H__ */