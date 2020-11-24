#include <stdio.h>
void print(const char *f1) {
    FILE *f1p;
    FILE *f2p;
    int line = 0;
    f1p = fopen(f1, "r");
    char ch1;

    printf("\n\n%s\n0  ", f1);
    do {
        ch1 = fgetc(f1p);
        printf("%c", ch1);
        if (ch1 == '\n')line++;
    } while (ch1 != EOF);


    do {
        if (ch1 == '\n') {
            line++;
            printf("%d  ", line);
        }
    } while (ch1 != EOF);

}

int main(int argc, char *argv[]) {
    while (argc >= 0) {
        print(argv);
        argc--;
    }
}
