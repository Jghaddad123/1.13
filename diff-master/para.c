#include <stdlib.h>
#include <string.h>
#include "para.h"
#include "util.h"

#define BUFLEN 256
static char buf[BUFLEN];

para* para_make(char* base[], int filesize, int start, int stop) {
    para* p = (para*) malloc(sizeof(para));
    p->base = base;
    p->filesize = filesize;
    p->start = start;
    p->stop = stop;
    p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
    p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];
    return p;
}
para* para_first(char* base[], int size) {
    para* p = para_make(base, size, 0, -1);
    return para_next(p);
}
void para_destroy(para* p) { free(p); }
para* para_next(para* p) {
    int i;
    if (p == NULL || p->stop == p->filesize) { return NULL; }
    para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
    for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
    pnew->stop = i;
    if (pnew->start >= p->filesize) {
        free(pnew);pnew = NULL;
    }
    return pnew;
}
size_t para_filesize(para* p) { return p == NULL ? 0 : p->filesize; }
size_t para_size(para* p) { return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; }
char** para_base(para* p) { return p->base; }
char* para_info(para* p) {
    snprintf(buf, sizeof(buf), "Base: %p, Size: %d, Start: %d, Stop: %d\n",
             p->base, p->filesize, p->start, p->stop);
    return buf;
}
int para_equal(para* p, para* q) {
    if (p == NULL || q == NULL) { return 0; }
    if (para_size(p) != para_size(q)) { return 0; }
    if (p->start >= p->filesize || q->start >= q->filesize) { return 0; }
    int i = p->start, j = q->start, equal = 0;
    while ((equal = strcmp(p->base[i], q->base[i])) == 0) { ++i; ++j; }
    return 1;
}
void para_print(para* p, void (*fp)(const char*)) {
    if (p == NULL) { return; }
    for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i]); }
}
void para_printfile(char* base[], int count, void (*fp)(const char*)) {
    para* p = para_first(base, count);
    while (p != NULL) {
        para_print(p, fp);
        p = para_next(p);
    }
    printline();
}

void normal(para* p,para* q) {
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printleftc);
                q = para_next(q);
                qlast = q;
            }
            para_print(q, printleftb);
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, printleftb);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printleftc);
        q = para_next(q);
    }
}
void _sidebyside(para* p, para* q){
    int foundmatch = 0; para* qlast = q;
    while (p != NULL) {
        qlast = q; foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;
        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printright); q = para_next(q); qlast = q;
            }
            para_print(q, printboth);
            p = para_next(p);  q = para_next(q);
        } else {
            para_print(p, printleft);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printright);
        q = para_next(q);
    }
}

void _leftcolumn(para* p,para* q) {
    int foundmatch = 0; para* qlast = q;
    while (p != NULL) {
        qlast = q; foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;
        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printright);
                q = para_next(q);  qlast = q;
            }
            para_print(q, printbothb);
            p = para_next(p);  q = para_next(q);
        } else {
            para_print(p, printleft);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printright);
        q = para_next(q);
    }
}

void _unified(para* p,para* q) {
    int foundmatch = 0;  para* qlast = q;
    while (p != NULL) {
        qlast = q;   foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;
        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printleftp);
                q = para_next(q);  qlast = q;
            }
            para_print(q, printleftm);
            p = para_next(p);   q = para_next(q);
        } else {
            para_print(p, printleftm);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printleftp);
        q = para_next(q);
    }
}
void _context(para* p,para* q) {
    int foundmatch = 0; para* qlast = q;
    while (p != NULL) {
        qlast = q; foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;
        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printleftp);
                q = para_next(q);   qlast = q;
            }
            para_print(q, printleftequal);
            p = para_next(p);  q = para_next(q);
        } else {
            para_print(p, printleftm);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printleftp);
        q = para_next(q);
    }
}
void _suppresscommon(para* p,para* q){
    int foundmatch = 0; para* qlast = q;
    while (p != NULL) {
        qlast = q; foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
            q = para_next(q);
        }
        q = qlast;
        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, printright);
                q = para_next(q);  qlast = q;
            }
            p = para_next(p); q = para_next(q);
        } else {
            para_print(p, printleft);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, printright);
        q = para_next(q);
    }
}


void printleft(const char *left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '<';
    buf[len + j++] = '\0';
    printf("%s\n", buf);
}
void printleftb(const char* left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '\0';
    printf("< %s\n", buf);
}
void printleftc(const char* left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '\0';
    printf("> %s\n", buf);
}

void printright(const char *right) {
    if (right == NULL) { return; }
    printf("%50s %s", ">", right);
}

void printboth(const char *left_right) {
    char buf[BUFLEN];
    size_t len = strlen(left_right);
    if (len > 0) { strncpy(buf, left_right, len); }
    buf[len - 1] = '\0';
    printf("%-50s %s", buf, left_right);
}
void printbothb(const char* left_right) {
    char buf[BUFLEN];
    strcpy(buf, left_right);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '(';
    buf[len + j++] = '\0';
    printf("%s\n", buf);
}


void printleftequal(const char* left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '\0';
    printf(" %s\n", buf);
}
void printleftm(const char* left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '\0';
    printf("- %s\n", buf);
}
void printleftp(const char* left) {
    char buf[BUFLEN];
    strcpy(buf, left);
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
    buf[len + j++] = '\0';
    printf("+ %s\n", buf);
}
