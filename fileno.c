#include <stdio.h>



int main(int argc, char** argv) {
    printf("%d\n", fileno(stdin));
    printf("%d\n", fileno(stdout));
    printf("%d\n", fileno(stderr));

    return 0;
}