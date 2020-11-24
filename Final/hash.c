#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define HASHSIZE 13

typedef struct hash_node {
  struct hash_node* next;
  const char* name;
  char* defn;
} hash_node;
static hash_node* hashtab[HASHSIZE];

unsigned hash(const char* s) {
  unsigned hashval = 0;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval % HASHSIZE;
}



hash_node* hash_lookup(const char* s) {  //COMPLETE
  struct hash_node *np;
  for(np = hashtab[hash(s)]; np != NULL; np = np->next)
      if(strcmp(s,np->name==0))
          return np;
      return NULL;

}


hash_node* hash_install(const char* name, const char* defn) {
  if (name == NULL) { return NULL; }
  hash_node* np;
  unsigned hash_idx;
  if ((np = hash_lookup(name)) == NULL) { /* name not found, need new node */
    np = (hash_node*)malloc(sizeof(hash_node));
    if (np == NULL || (np->name = strdup(name)) == NULL) { return NULL; }
    hash_idx = hash(name);
    np->next = hashtab[hash_idx];
    hashtab[hash_idx] = np;
  } else {
    free((void*)np->defn);
  }
  if ((np->defn = strdup(defn)) == NULL) { return NULL; }
  return np;
}

void hash_deletenode(hash_node* p) {  // Complete
    free(p);
}

void hash_clear() {  // Complete
    for(int i = 0;i<HASHSIZE;i++) {
        hash_undef(hashtab[i]->name);
    }
}

void hash_undef(const char* name) {
  if (name == NULL) { return; }
  hash_node* np;
  unsigned hash_idx;
  if ((np = hash_lookup(name)) == NULL) { return; }
  hash_idx = hash(name);
  hash_node* p = np;
  hash_node* prev = np;
  while (p != np) {
    prev = p;
    p = p->next;
  }
  prev->next = p->next;
  hash_deletenode(p);
  if (hashtab[hash_idx]->next == NULL) { hashtab[hash_idx] = NULL; }
}
const char* yesorno(bool cond) { return cond ? "YES" : "no"; }

void print_defn(const char* s) {
  hash_node* p = hash_lookup(s);
  printf("'%s': ", s);
  printf("%s ", p ? p->defn : "not found");
  printf(" ---> %p\n",  p ? p->next : NULL);
}


void hash_print(void) {  // Complete
        for(int i = 0;i<HASHSIZE;i++){
            if(hashtab[i]!=NULL){
                hash_node *ptr = hashtab[i];
                while(ptr!=NULL){
                    printf("Name: %s defn: %s\n",ptr->name,ptr->defn);
                    ptr = ptr->next;
                }
            }
        }
}


void test_hash_table() {
  hash_install("trombone", "bass brass wind instrument with slide");
  hash_install("trumpet", "tenor brass wind instrument with 3 valves");
  print_defn("trombone");
  print_defn("trumpet");
  printf("\n");
  hash_install("choo-choo", "train");
  hash_install("chattanooga", "pretty city in southern Tennessee");
  hash_undef("trombone");
  hash_print();
}
int main(int argc, const char * argv[]) {
  test_hash_table();
  return 0;
}
