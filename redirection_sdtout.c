#include <stdio.h>
#include <stdlib.h>

void rediriger(char * filename){
    printf("Avant redirection\n");
    FILE* file = freopen(filename, "w", stdout);

    if (file == NULL) {
        perror("Error while opening file\n");
        exit(EXIT_FAILURE);
    }
    printf("Après redirection\n");
    int closeFile = fclose(file);
    if(closeFile < 0){
        perror("Error while closing file\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv){
    rediriger(argv[1]);

    return 0;
}