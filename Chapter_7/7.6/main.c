#include <stdio.h>

void print(const char *f1,const char *f2,int value){
    FILE *f1p;
    FILE *f2p;
    int line=0;
    f1p = fopen(f1, "r");
    f2p = fopen(f2, "r");char ch1, ch2;

    printf("They differ on line %d printing each file at difference\n", value);
    do {
        ch1 = fgetc(f1p);
        if(value==line){
            printf("%c", ch1);
        }
        if (ch1 == '\n')line++;
        } while (ch1 != EOF);


    do {
        ch2 = fgetc(f2p);
        if(value==line){
            printf("%c", ch2);
        }
        if (ch2 == '\n')line++;
    } while (ch2 != EOF);
    
}
int different(const char *f1, const char *f2) {
    FILE *f1p;
    FILE *f2p;
    int line;
    f1p = fopen(f1, "r");
    f2p = fopen(f2, "r");
    char ch1, ch2;
    do {
        ch1 = fgetc(f1p);
        ch2 = fgetc(f2p);
        if (ch1 == '\n')line++;
        if (ch1 != ch2)
            return line;
    } while (ch1 != EOF && ch2 != EOF);
    if (ch1 == EOF && ch2 == EOF)return -1;
    else return line;
}
int main(int argc, char* argv[]) {
int value = different(argv[0],argv[1]);
print(argv[0],argv[1],value);
}
