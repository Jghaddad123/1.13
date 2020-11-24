#include "slist.h"



snode* snode_create(tnode* node, snode* next) {
    snode* p = (snode*)malloc(sizeof(snode));
    p->node = node;
    p->next = next;
    return p;
}
slist* slist_create(void) {
    slist* p = (slist*)malloc(sizeof(slist));
    p->head = NULL;
    p->tail = NULL;
    p->size = 0;
    return p;
}

tnode* slist_front(slist* list) {
    return list->head->node;
}
bool slist_empty(slist* list) {return (list->size == 0);}
size_t slist_size(slist* list) {return list->size;}
void slist_clear(slist* list) {while(!slist_empty(list)) slist_pop(list);}

void slist_pop(slist* list) {
    if (slist_empty(list)) {fprintf(stderr, "Underflow error\n");    return;}
    snode *p = list->head;
    list->head = list->head->next;
    if (list->head == NULL) list->tail = NULL;
    --list->size;    free(p);
}
void slist_push(slist* list, tnode* node) {
        snode* p = snode_create(node, NULL);
    if (list->size != 0) list->tail->next = p;
    else list->head = p;
    list->tail = p;
    ++list->size;
}



