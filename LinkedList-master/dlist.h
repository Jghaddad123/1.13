//
// Created by Joseph on 4/10/2020.
//

#ifndef NEWLINKEDLIST_DLIST_H
#define NEWLINKEDLIST_DLIST_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
typedef struct dnode dnode;
typedef struct dlist dlist;


dnode* dnode_create(int data, dnode* prev, dnode* next);
dlist* dlist_create(void);
bool dlist_empty(dlist* list);
size_t dlist_size(dlist* list);
void dlist_popfront(dlist* list);
void dlist_popback(dlist* list);
void dlist_pushfront(dlist* list, int data);
void dlist_pushback(dlist* list, int data);
void dlist_clear(dlist* list);
void dlist_print(dlist* list, const char* msg);
int dlist_front(dlist* list);
int dlist_back(dlist* list);
void dlist_test(void);
const char* yesorno(bool condition);
void dlist_test_loop();
static void dlist_testset(dlist* list, int start, int end, int pattern, void (*fp)(dlist*, int));

#endif //NEWLINKEDLIST_DLIST_H
