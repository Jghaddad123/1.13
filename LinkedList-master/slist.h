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
typedef struct snode snode;
typedef struct slist slist;

snode* snode_create(int data, snode* next);
slist* slist_create(void);
bool slist_empty(slist* list);
size_t slist_size(slist* list);
void slist_popfront(slist* list);
void slist_popback(slist* list);
void slist_pushfront(slist* list, int data);
void slist_pushback(slist* list, int data);
void slist_clear(slist* list);
void slist_print(slist* list, const char* msg);
int slist_front(slist* list);
int slist_back(slist* list);
void slist_test(void);
void slist_test_loop();
const char* yesorno(bool condition);
static void slist_testset(slist* list, int start, int end, int pattern, void (*fp)(slist*, int));

#endif //NEWLINKEDLIST_SLIST_H
