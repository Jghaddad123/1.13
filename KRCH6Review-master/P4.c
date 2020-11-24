#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct tnode {
    char* word;
    int freq;
    struct tnode* left;
    struct tnode* right;
};
typedef struct tnode tnode;
struct snode{
    char* word;
    int freq;
    struct snode* next;
};
typedef struct snode snode;
struct slist{
    snode* head;
    snode* tail;
    size_t size;
};
typedef struct slist slist;
tnode* tree_create();
void treeprint(tnode*);
char* getword();
int getch(void);
void ungetch(int);
tnode* talloc(void);
snode* snode_create(char*, snode*);
slist* slist_create(void);
size_t slist_size(slist*);
void slist_pushfront(slist*, char*);
void slist_print(snode*);
bool search_slist(slist*, char*, snode* );
bool slist_empty(slist*);

slist* list;
char buf[BUFSIZ];
int buff = 0;

int main(void){
    tnode *root;
    char *word;

    list = slist_create();
    root = NULL;
    while ((word = getword()) != NULL)
        if (isalpha(word[0]))
            slist_pushfront(list, word);
    root = tree_create(root, list->head);
    treeprint(root);
    slist_print(list->head);
    return 0;
}
tnode* tree_create(tnode* pt, snode* current){
    if(current == NULL){ return pt;}
    else if (pt == NULL) {
        pt = talloc();
        pt->word = strdup(current->word);
        pt->freq = current->freq;
        pt->left = pt->right = NULL;
    }
    else if(pt->freq >= current->freq){pt->right = tree_create(pt->right, current->next);}
    else{pt->left = tree_create(pt->left, current->next);}
    return pt;
}
void slist_print(snode* curr){
    printf("%dx: [%s]\n", curr->freq, curr->word);
    slist_print(curr->next);
}
void treeprint(tnode* pt){
    int count = 0;
    if (pt != NULL) {
        printf("%d time\n", count);
        treeprint(pt->left);
        printf("Number of occurences %d : [%s]\n", pt->freq, pt->word);
        treeprint(pt->right);
    }
}
tnode* talloc(void){
    return (tnode*) malloc(sizeof(tnode));
}
snode* snode_create(char* word, snode* next){
    snode* np = (snode*)malloc(sizeof(snode));
    np->word = word;
    np->freq = 1;
    np->next = next;
    return np;
}
slist* slist_create(){
    slist* np = (slist*)malloc(sizeof(slist));
    np->head = NULL;
    np->tail = NULL;
    np->size = 0;
    return np;
}
bool slist_empty(slist* list){
    if(list->head == NULL){return true;}
    return false;
}
size_t slist_size(slist* list){
    return list->size;
}
bool search_slist(slist* list, char* word, snode* current){
    if(current == NULL){ return false;}
    if(strcmp(current->word, word) == 0){
        current->freq++;
        return true;
    }
    search_slist(list, word, current->next);
    return false;
}
void slist_pushfront(slist* list, char* word){
    if(search_slist(list, word, list->head)){ return;}
    else{
        snode* no = snode_create(word, list->head);
        no->next = list->head;
        list->head = no;
        if(list->size == 0){list->tail = no;}
        ++list->size;
    }
}
char* getword(){
    static char word[100];
    int c, lim = 100;
    char* w = word;

    while (isspace(c = getch()) && c != '\n');
    if (c != EOF)  *w++ = tolower(c);
    if (isalpha(c)) {
        while(--lim > 1){
            if (!isalnum(*w = tolower(getch())) && *w != '_') {ungetch(*w);break;}
            w++;
        }
    }
    *w = '\0';
    if (c == EOF)return NULL;
    else return word;
}
int getch(void){
    return(buff > 0) ? buf[--buff] : getchar();
}
void ungetch(int c){
    if(buff >= BUFSIZ){ fprintf(stderr, "Stack Overflow");}
    else{ buf[buff++] = c;}
}
