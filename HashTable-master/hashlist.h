//
// Created by Joseph on 4/15/2020.
//

#ifndef HASHTABLE_HASHLIST_H
#define HASHTABLE_HASHLIST_H
#define HASHSIZE 101
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct hash_node {
    struct hash_node* next;
    const char* name;
    char* defn;
} hash_node;

unsigned hash(const char* s);
void hash_clear();
hash_node* hash_install(const char* name, const char* defn);
hash_node* hash_lookup(const char* s);
static hash_node* hashtab[HASHSIZE];
void print_defn(const char* s);

#endif //HASHTABLE_HASHLIST_H
