//
// Created by Joseph on 4/10/2020.
//

#ifndef NEWLINKEDLIST_SLIST_H
#define NEWLINKEDLIST_SLIST_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "tree.h"

typedef struct snode snode;
typedef struct slist slist;
struct snode {
    tnode* node;
    snode* next;
};
struct slist {
    snode* head;
    snode* tail;
    size_t size;
};
snode* snode_create(tnode* node, snode* next);
slist* slist_create(void);
bool slist_empty(slist* list);
size_t slist_size(slist* list);
void slist_pop(slist* list);
void slist_push(slist* list, tnode* node);
void slist_clear(slist* list);
tnode* slist_front(slist* list);

#endif //NEWLINKEDLIST_SLIST_H
