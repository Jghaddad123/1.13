#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 12          
static char* noises[] = {"a", "an", "and", "are", "for", "from", "in", "is", "it", "not", "of", "on", "or", "that", "the", "this", "to", "was", "with"};
struct tnode {
    char *word;
    struct tnode *left;
    struct tnode *right;
    unsigned lines[MAXLINES];
};
typedef struct tnode tnode;
tnode* addtree(tnode*, char*, int);
void treeprint(tnode*);
char* getword();
int noise_check(char*);
int getch(void);
void ungetch(int);
tnode* talloc(void);
char buf[BUFSIZ];
int bufp = 0;

int main(void){
    tnode *root;
    char *word;
    unsigned int line = 1;

    root = NULL;
    while ((word = getword()) != NULL)
        if (isalpha(word[0]) && !noise_check(word))
            root = addtree(root, word, line);
        else if (word[0] == '\n')
            line++;
    treeprint(root);
    return 0;
}

int noise_check(char* word){
    int cond;
    int low, high, avg;

    if (word[1] == '\0'){return 1;}
    low = 0;
    high = sizeof(noises) / sizeof(char *) - 1;
    while (low <= high) {
        avg = (low + high) / 2;
        if ((cond = strcmp(word, noises[avg])) < 0) {high = avg - 1;}
        else if (cond > 0) {low = avg + 1;}
        else {return 1;}
    }
    return 0;
}

tnode* addtree(tnode* pt, char* word, int ln){
    int cond;
    int i;

    if (pt == NULL) {
        pt = talloc();
        pt->word = strdup(word);
        pt->lines[0] = ln;
        for (i = 1; i < MAXLINES; i++)
            pt->lines[i] = 0;
        pt->left = pt->right = NULL;
    }
    else if ((cond = strcmp(word, pt->word)) == 0) {
        for (i = 0; pt->lines[i] && i < MAXLINES; i++);
        pt->lines[i] = ln;
    }
    else if (cond < 0) {pt->left = addtree(pt->left, word, ln);}
    else if (cond > 0) {pt->right = addtree(pt->right, word, ln);}
    return pt;
}

void treeprint(tnode* pt){
    int i;

    if (pt != NULL) {
        treeprint(pt->left);
        printf("%-16s", pt->word);
        putchar('[');
        for (i = 0; i < MAXLINES && pt->lines[i]; i++)
            printf("%s%d", i==0 ? "" : ", ",pt->lines[i]);
        putchar(']');
        putchar('\n');
        treeprint(pt->right);
    }
}

char* getword(){
    static char word[100];
    int c, lim = 100;
    char* w = word;

    while (isspace(c = getch()) && c != '\n');
    if (c != EOF) *w++ = tolower(c);
    if (isalpha(c)) {
        while(--lim > 1){
            if (!isalnum(*w = tolower(getch())) && *w != '_') {  ungetch(*w);break;}
            w++;
        }
    }
    *w = '\0';
    if (c == EOF)
        return NULL;
    else
        return word;
}
int getch(void){
    return(bufp > 0) ? buf[--bufp] : getchar();
}
void ungetch(int c){
    if(bufp >= BUFSIZ){ fprintf(stderr, "Stack Overflow");}
    else{ buf[bufp++] = c;}
}
tnode* talloc(void){
    return (tnode*) malloc(sizeof(tnode));
}
