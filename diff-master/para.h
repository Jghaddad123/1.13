#ifndef para_h
#define para_h

#include <stdio.h>

typedef struct para para;
struct para {
    char** base;
    int filesize;
    int start;
    int stop;
    char* firstline;
    char* secondline;
};

para* para_make(char* base[], int size, int start, int stop);
para* para_first(char* base[], int size);
para* para_next(para* p);
size_t para_filesize(para* p);
size_t para_size(para* p);
char** para_base(para* p);
char* para_info(para* p);
int   para_equal(para* p, para* q);
void para_print(para* p, void (*fp)(const char*));
void para_printfile(char* base[], int count, void (*fp)(const char*));

void printleft(const char *left);
void printleftc(const char* left);
void printleftb(const char* left);
void printright(const char *right);
void printboth(const char *left_right);
void printbothb(const char* left_right);

void printleftm(const char* left);
void printleftequal(const char* left);
void printleftp(const char* left);

void normal(para *p, para *q);
void _sidebyside(para* p, para* q);
void _leftcolumn(para* p,para* q);
void _suppresscommonl(para* p,para* q);
void _context(para* p,para* q);
void _unified(para* p,para* q);

#endif /* para_h */
