#ifndef __ADT_H__
#define __ADT_H__

#include "arch/defs.h"

typedef struct _PRIORITY_QUEUE{
    void *data;
    struct _PRIORITY_QUEUE *left;
    struct _PRIORITY_QUEUE *right;
} PRIORITY_QUEUE;

typedef struct _LOOP_LIST{
    void *data;
    struct _LOOP_LIST *next;
} LOOP_LIST;

LOOP_LIST * list_add(LOOP_LIST * list, void * data);

#endif /* __ADT_H__ */