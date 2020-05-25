#include <stdio.h>
#include <fcntl.h>

//
// Created by arsen on 25/05/2020.
//
void rediriger(char * filename){
    printf("Avant redirection");
    freopen("redirect.txt", "w", stdout);
    printf("Après redirection");
}

int main(int argc, char** argv){
    rediriger(argv[1]);

    return 0;
}