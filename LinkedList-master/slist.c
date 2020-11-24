#include "slist.h"

struct snode {
    int data;
    snode* next;
};
struct slist {
    snode* head;
    snode* tail;
    size_t size;
};

snode* snode_create(int data, snode* next) {
    snode* p = (snode*)malloc(sizeof(snode));
    p->next = next;
    p->data = data;
    return p;
}
slist* slist_create(void) {
    slist* p = (slist*)malloc(sizeof(slist));
    p->head = NULL;
    p->tail = NULL;
    p->size = 0;
    return p;
}

int slist_front(slist* list) {
    return list->head->data;
}
int slist_back(slist* list) {
    return list->tail->data;
}

const char* yesorno(bool condition) {return condition ? "YES" : "NO";}
bool slist_empty(slist* list) {return (list->size == 0);}
size_t slist_size(slist* list) {return list->size;}
void slist_clear(slist* list) {while(!slist_empty(list)) slist_popfront(list);}

void slist_popfront(slist* list) {
    if (slist_empty(list)) {fprintf(stderr, "Underflow error\n");    return;}
    snode *p = list->head;
    list->head = list->head->next;
    if (list->head == NULL) list->tail = NULL;
    --list->size;    free(p);
}
void slist_popback(slist* list) {
    if (slist_empty(list)) {fprintf(stderr, "Underflow error\n");   return;}
    snode* p = list->head;
    while(p->next != list->tail) p = p->next;
    p->next = NULL;
    free(list->tail);
    list->tail = p;
}

void slist_pushfront(slist* list, int data) {
    snode* node = snode_create(data, NULL);
    node->next = list->head;
    list->head = node;
    if (list->size == 0) {list->tail = node;}
    ++list->size;
}
void slist_pushback(slist* list, int data) {
    if (list->size == 0) {slist_pushfront(list, data);     return;}
    snode* node = snode_create(data, NULL);
    node->next = NULL;
    list->tail->next = node;
    list->tail = node;
    ++list->size;
}

void slist_print(slist* list, const char * msg) {
    if (slist_empty(list)) {
        fprintf(stderr, "Underflow error\n");
        return;
    }
    printf("%s\n", msg);
    snode* p = list->head;
    while(p != NULL) {
        printf("%d --> %p\n", p->data,p);
        p = p->next;
    }
    printf("\n");
}






static void slist_testset(slist* list, int start, int end, int pattern, void (*fp)(slist*, int)) {
    for (int i = start; i < end; i += pattern) {
        fp(list, i);
    }
    slist_print(list, "list is created and is...");
    printf("list size is: %d\n", slist_size(list));
    printf("list is empty? %s\n", yesorno(slist_empty(list)));

    int fin = start;
    while ((fin += pattern) < (end-pattern)) {}
    if (fp == slist_pushback)  {
        printf("assert front of list is: %d\n", start);
        assert(slist_front(list) == start);
        printf("assert back of list is: %d\n", fin);
        assert(slist_back(list) == fin);
    }
    if (fp == slist_pushfront) {
        printf("assert front of list is: %d\n", fin);
        assert(slist_front(list) == fin);
        printf("assert back of list is: %d\n", start);
        assert(slist_back(list) == start);
    }

    slist_clear(list);
    printf("after clearing the list, is the list is now empty? %s\n", yesorno(slist_empty(list)));
}

//-------------------------------------------------
void slist_test_loop() {
    printf("\n===================== TESTING SLIST =====================\n");
    slist* list = slist_create();
    slist_testset(list, 10, 50, 10, slist_pushfront);
    slist_testset(list, 10, 100, 20, slist_pushback);
    free(list);

    printf("\n    All Assertions passed!...\n");
    printf("===================== END TESTING SLIST =====================");
}


