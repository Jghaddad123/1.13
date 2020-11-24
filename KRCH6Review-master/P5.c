void undef(char *s)
{
    struct nlist *np, *np2;
    unsigned hashval = hash(s);

    for (np = hashtab[hashval], np2 = NULL;
         np2 != NULL;
         np2 = np, np = np->next)
        if (strcmp(s, np->name) == 0) {
            free(np->name);
            free(np->defn);
            if (np2 == NULL)
                hashtab[hashval] = np->next;
            else
                np2->next = np->next;
            free(np);
            return;
        }
}
