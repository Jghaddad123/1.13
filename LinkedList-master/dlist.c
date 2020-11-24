#include "dlist.h"
struct dnode {
    int data;
    dnode* prev;
    dnode* next;
};
struct dlist {
    dnode* head;
    dnode* tail;
    size_t size;
};

dnode* dnode_create(int data,dnode* prev, dnode* next) {
    dnode* p = (dnode*)malloc(sizeof(dnode));
    p->prev = prev;
    p->next = next;
    p->data = data;
    return p;
}
dlist* dlist_create(void) {
    dlist* p = (dlist*)malloc(sizeof(dlist));
    p->head = NULL;
    p->tail = NULL;
    p->size = 0;
    return p;
}

int dlist_front(dlist* list) {
    return list->head->data;
}
int dlist_back(dlist* list) {
    return list->tail->data;
}
const char* yesorno1(bool condition) {return condition ? "YES" : "NO";}
bool dlist_empty(dlist* list) {return (list->size == 0);}
size_t dlist_size(dlist* list) {return list->size;}
void dlist_clear(dlist* list) {while(!dlist_empty(list)) dlist_popfront(list);}



//NULL <-- dnode <--> head <--> tail --> NULL
//NULL <-- head <--> tail --> NULL
void dlist_popback(dlist* list) {
    if (dlist_empty(list)) {fprintf(stderr, "Underflow error\n");    return;}
    dnode *p = list->tail;
    list->tail = list->tail->prev;
    if (list->tail == NULL) list->head = NULL;
    else list->tail->next =NULL;
    --list->size;    free(p);
}
//NULL <-- head <--> tail <--> dnode --> NULL
//NULL <-- head <--> tail --> NULL
void dlist_popfront(dlist* list) {
    if (dlist_empty(list)) {fprintf(stderr, "Underflow error\n");   return;}
    dnode* p = list->head;
    list->head = list->head->next;
    if (list->head == NULL) list->tail = NULL;
    else list->head->prev =NULL;
    --list->size;    free(p);
}

void dlist_pushfront(dlist* list, int data) {
    dnode* node = dnode_create(data,NULL, list->head);
    if (list->size != 0) list->head->prev = node;
    list->head = node;
    if (list->size == 0) {list->tail = node;}
    ++list->size;
}
void dlist_pushback(dlist* list, int data) {
    if (list->size == 0) {dlist_pushfront(list, data);     return;}
    dnode* node = dnode_create(data, list->tail, NULL);
    list->tail->next = node;
    list->tail = node;
    ++list->size;
}

void dlist_print(dlist* list, const char * msg) {
    if (dlist_empty(list)) {
        fprintf(stderr, "Underflow error\n");
        return;
    }
    printf("%s\n", msg);
    dnode* p = list->head;
    while(p != NULL) {
        printf("%p <-- %2d %p -->%p \n", p->prev,p->data,p,p->next);
        p = p->next;
    }
    printf("\n");
}

static void dlist_testset(dlist* list, int start, int end, int pattern, void (*fp)(dlist*, int)) {
    for (int i = start; i < end; i += pattern) {
        fp(list, i);
    }
    dlist_print(list, "list is created and is...");
    printf("list size is: %d\n", dlist_size(list));
    printf("list is empty? %s\n", yesorno(dlist_empty(list)));

    int fin = start;
    while ((fin += pattern) < (end-pattern)) {}
    if (fp == dlist_pushback)  {
        printf("assert front of list is: %d\n", start);
        assert(dlist_front(list) == start);
        printf("assert back of list is: %d\n", fin);
        assert(dlist_back(list) == fin);
    }
    if (fp == dlist_pushfront) {
        printf("assert front of list is: %d\n", fin);
        assert(dlist_front(list) == fin);
        printf("assert back of list is: %d\n", start);
        assert(dlist_back(list) == start);
    }

    dlist_clear(list);
    printf("after clearing the list, is the list is now empty? %s\n", yesorno(dlist_empty(list)));
}

//-------------------------------------------------
void dlist_test_loop() {
    printf("\n===================== TESTING DLIST =====================\n");
    dlist* list = dlist_create();
    dlist_testset(list, 0, 100, 1, dlist_pushback);
    free(list);

    printf("\n    All Assertions passed!...\n");
    printf("===================== END TESTING DLIST =====================");
}
