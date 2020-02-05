#include <stdio.h>
#include <string.h>

void find_word_length(void (*f)(int[]));

void printout() {
    for (int i = 1; i <= 24; i++) {
        printf(" %2d: ", i);
    }
}

void print_horiz_histogram(int length[]) {
    for (int j = 1; j < 25; j++) {
        if ((j-1) % 4 == 0)
            printf("---------------------------------------------------------------------------------------------------------------------- \n");
        if (length[j] == 0)
            printf("  %2d:", j);


        for (int i = 0; i < length[j]; i++) {
            if (i == 0) {
                printf("  %2d: ", j);
            } else if (i % 5 == 0) {
                printf("|");
            }
            printf("*");
        }

        printf("\n");
    }
}

void print_vert_histogram(int length[]) {
    int max = 0;
    for (int i =0; i<25;i++){
        if(length[i]>max)
            max = length[i];
    }
    max+=3;
    printf("  ======================================================================================================================");
    printout();
    printf("  ======================================================================================================================\n");


    for (int j = 1; j < max; j++) {
        for (int i = 1; i < 25; i++) {
            if(length[i]>=j){
                printf("  *  ");
            }
            else
                printf("  .  ");
        }
       // printf("\n");
        if ((j) % 5 == 0&&(j!=0))
            printf("  ----------------------------------------------------------------------------------------------------------------------");

    }



}


//-----------------------------------------------------------
void find_word_length(void (*f)(int[])) {   // pre-C99 -- have to write void if no args

    int length[25];
    for (int i = 0; i < 25; i++) {
        length[i] = 0;
    }
//    memset(length, 0, sizeof(length));  DIDNT WORK AND SIZEOF RETURNED 92

    int c;
    int posfirst = 0;
    int poslast = 0;
    char last = 'a';
    while ((c = getchar()) != '-') {

        if (((last == ' ') || (last == '\n')) && ((c == '\n') || (c == ' '))) {
            posfirst = poslast;
        }
        else if ((c == ' ' || c == '\n') && (posfirst == 0)) {
            length[(poslast - posfirst)]++;
            posfirst = poslast;
        }
        else if (c == ' ' || c == '\n' || c == EOF) {
            length[(poslast - (posfirst + 1))]++;
            posfirst = poslast;
        }

        last = c;
        ++poslast;
    }

    (*f)(length);
}

int main(int argc, const char *argv[]) {
   find_word_length(print_horiz_histogram);
   find_word_length(print_vert_histogram);


    return 0;
}
