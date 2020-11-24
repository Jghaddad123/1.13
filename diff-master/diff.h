#ifndef DIFF_DIFF_H
#define DIFF_DIFF_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXSTRINGS 2048
#define BUFLEN 256

bool different(const char* f1,const char* f2);
bool checkequal(char *string1, char *string2);
void loadfile(const char* filename1, const char* filename2);



bool version = false, brief = false, report_ident_files = false,
     report_ident_or_diff_files = false, standard = true,
     sidebyside = false, left_column = false, suppress_common_lines = false,
     context = false, unified = false;
static char buf[BUFLEN];
static char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
static int index1 = 0, index2 = 0;
double versionNum = 1.03;
char str1[100];
char newString[10][4];
FILE *f1p;
FILE *f2p;

#endif //DIFF_DIFF_H
