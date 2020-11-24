//
// Created by Joseph on 4/15/2020.
//

#include "hashlist.h"

unsigned hash(const char* s){
    unsigned hashval;
    for(hashval=0;*s !='\0';s++){hashval = *s+31*hashval;}
    return hashval%HASHSIZE;
}
hash_node* hash_lookup(const char* s){
    hash_node *np;
    for(np = hashtab[hash(s)]; np!=NULL; np->next)
            if(strcmp(s,np->name)==0) return np;
    return NULL;
}
hash_node* hash_install(const char* name, const char* defn){
    hash_node *np;
    unsigned hashval;

    if((np = hash_lookup(name))== NULL) {
        np = (hash_node*) malloc(sizeof(*np));
        if (np==NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }else{
        free((void*) np->defn);}
    if((np->defn = strdup(defn))==NULL)
        return NULL;
    return np;

    }
void hash_clear(){

}


void test_hash_table() {
    printf("%s\n", "// ---------------------- TESTING HASH_TABLE ---------------------");
    hash_install("cat", "animal that likes fish");
    hash_install("dog", "animal that hates cats");
    hash_install("mouse", "animal eaten by cats");
    hash_install("virus", "pain in the neck");

    print_defn("cat");
    print_defn("pig");
    print_defn("dog");
    print_defn("pig");
    print_defn("mouse");
    print_defn("bacteria");
    print_defn("amoeba");
    print_defn("paramecium");
    print_defn("virus");


    printf("\nredefining cat...\n");
    printf("redefining virus...\n");
    hash_install("cat", "animal that likes fish and mice and birds");
    hash_install("virus", "VERY EXPENSIVE pain in the neck");
    print_defn("cat");
    print_defn("virus");

    hash_clear();
    printf("\n%s\n", "// ----------------END OF TESTING HASH_TABLE ---------------------");
}
void print_defn(const char* s) {
    hash_node* p = hash_lookup(s);
    printf("'%s': ", s);
    printf("%s\n", p ? p->defn : "not found");
}

int main(int argc, const char * argv[]) {
    test_hash_table();


    return 0;
}


