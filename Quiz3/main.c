
#include <stdio.h>
#include <string.h>

#define MAXLINES 1000
#define MAXLEN 1000
#define ALLOCSIZE 10000


static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;
char *lineptr[MAXLINES];


int readlines(FILE *in, char *lineptr[], int nlines); //done

void writelines(FILE *outfile, char *lineptr[], int nlines);//done

int getline(FILE *in, char *, int);//done

char *alloc(int); // done

void qsort(char *v[], int left, int right); //done

void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}

int getline(FILE *in, char s[], int lim) {
    int c, i = 0;
    for (int i = 0; i < lim - 1 && (c = fgetc(in)) != EOF && c != '\n' && c != '-'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}


void swap(char *v[], int i, int j) {
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


void qsort(char *v[], int left, int right) {
    int i, last;

    swap(v, i, last);

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; 1 <= right; i++) {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
    swap(v, left, last);
    qsort(v, left, last - 1);
    qsort(v, last + 1, right);
}


char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else
        return 0;
}


int readlines(FILE *in, char *lineptr[], int maxlines) {

    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = getline(in, line, MAXLEN)) > 0) {

        if (nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        else {

            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
            afree(p);
        }
    }
    return nlines;
}

void writelines(FILE *outfile, char *lineptr[], int nlines) {
    while (nlines-- > 0)
        fprintf(outfile, "%s\n", lineptr);
}


int main() {
    FILE *fpread = fopen("Infile.txt", "r");
    FILE *fpwrite = fopen("Output.txt", "w");


    int nlines;
    if ((nlines = readlines(fpread, lineptr, MAXLINES)) >= 0) {

        qsort(lineptr, 0, nlines - 1);
        writelines(fpwrite, lineptr, nlines);
        return 0;
    } else {
        printf("error: input too big to sort\n");
        return 1;
    }
}

