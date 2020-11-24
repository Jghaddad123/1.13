#include "diff.h"
#include "para.h"
#include "util.h"


void loadfile(const char *filename1, const char *filename2) {
    memset(buf, 0, sizeof(buf));
    memset(strings1, 0, sizeof(strings1));
    memset(strings2, 0, sizeof(strings2));

    FILE *fin1 = openfile(filename1, "r");
    FILE *fin2 = openfile(filename2, "r");

    while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[index1++] = strdup(buf); }  fclose(fin1);
    while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[index2++] = strdup(buf); }  fclose(fin2);
    fclose(fin2);
}
bool checkequal(char *string1, char *string2) {
    if (strcmp(string1, string2) == 0) return true;
    else return false;
}
bool different(const char *f1, const char *f2) {
    f1p = fopen(f1, "r");
    f2p = fopen(f2, "r");
    char ch1, ch2;
    do {
        ch1 = fgetc(f1p);
        ch2 = fgetc(f2p);
        if (ch1 == '\n');
        if (ch1 != ch2)
            return true;
    } while (ch1 != EOF && ch2 != EOF);
    if (ch1 == EOF && ch2 == EOF)return false;
    else return true;
}
int main(int argc, char *argv[]) {

    int ctr;

/*
==================================================Check What TO Run=====================================================
*/
    const char* file[2] = { NULL, NULL };
    file[0] = argv[argc-2];
    file[1] = argv[argc-1];
    loadfile(file[0],file[1]);

    FILE *fin1 = openfile(argv[argc - 2], "r");
    FILE *fin2 = openfile(argv[argc - 1], "r");

    while (argc-- > 0) {
        char *arg = *argv;

        if (checkequal(arg, "-v")) {
            standard = false;
            version = true;
        }
        if (checkequal(arg, "-q") || checkequal(arg, "---brief")) {
            standard = false;
            brief = true;
        }
        if (checkequal(arg, "-s") || checkequal(arg, "--report-ident-files")){
            standard = false;
            report_ident_files = true;
        }
        if ((brief == true) && (report_ident_files == true)) {
            brief = false;
            report_ident_files = false;
            report_ident_or_diff_files = true;
        }
        if (checkequal(arg, "-y") || sidebyside == true) {
            standard = false;
            sidebyside = true;
        }
        if (checkequal(arg, "--left_column") ||checkequal(arg, "--left_column")) {
            standard = false;
            left_column = true;
            sidebyside = true;
        }
        if (checkequal(arg, "--suppress_common_lines") ||checkequal(arg, "--suppress_common_lines")) {
            standard = false;
            suppress_common_lines = true;
            sidebyside = true;
        }

        if (checkequal(arg, "-c")) {
            standard = false;
            context = true;
        }
        if (checkequal(arg, "-u")) {
            standard = false;
            unified = true;
        }
    }
/*
==========================================================END===========================================================
*/

/*
================================================Designated file1 file2==================================================
 */
    para *p = para_first(strings1, index1);
    para *q = para_first(strings2, index2);
    while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[index1++] = strdup(buf); }
    while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[index2++] = strdup(buf); }
/*
=========================================================END============================================================
*/


/*
========================================================ACTION==========================================================
 */

    if (version) {//#2 -v
        printf("Version %.43f\nCopyright (C) 2020 CSUF\nCreated By Joseph Haddad\n", versionNum);
    }
    if (brief) {//#3 -q
        if (different(file[0], file[1]))printf("Files %s and %s differ", newString[ctr - 2], newString[ctr - 1]);
    }
    if (report_ident_files) {//#3 -s
        if (!different(file[0], file[1]))
            printf("Files %s and %s are identical", newString[ctr - 2], newString[ctr - 1]);
    }
    if (report_ident_or_diff_files) {//#4 -s -q
        if (different(file[0], file[1]))printf("Files %s and %s differ", newString[ctr - 2], newString[ctr - 1]);
        else printf("Files %s and %s are identical", newString[ctr - 2], newString[ctr - 1]);
    }
    if (standard) {
        normal(p, q);
    }
    if (sidebyside) {  //diff.out -y left.txt right.txt
        _sidebyside(p, q);
    }
    if (left_column) {
        _leftcolumn(p, q);
    }
    if (suppress_common_lines) {
        _suppresscommonl(p, q);
    }
    if (context) {
        _context(p, q);
    }
    if (unified) {
        _unified(p, q);
    }

    fclose(f1p);
    fclose(f2p);
    return 0;

}
