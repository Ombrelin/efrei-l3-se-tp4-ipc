//
// Created by Marion on 25/05/2020.
//

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main (int argc, char** argv) {

    if (argv[1] == NULL) {
        perror("Veuillez fournir un argument contenant un mot\n");
        exit(EXIT_FAILURE);
    }

    int mypipe[2];

    if (pipe(mypipe) < 0) {
        perror("Error opening pipe\n");
        exit(255);
    }

    int pid = fork();

    switch (pid) {
        case -1:
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        case 0:
            if (close(mypipe[0]) < 0) {
                perror("[SON] Error closing mypipe reading\n");
                exit(EXIT_FAILURE);
            }

            int number = strlen(argv[1]);
            int getPid = getpid();

            write(mypipe[1], &getPid, sizeof(int));
            write(mypipe[1], &number, sizeof(int));

            char* string = argv[1];

            for (int i = 0; i < strlen(string); i++) {
                write(mypipe[1], &(string[i]), sizeof(char));
            }

            if (close(mypipe[1]) < 0) {
                perror("[SON] Error closing mypipe writing\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            if (close(mypipe[1]) < 0) {
                perror("[FATHER] Error closing mypipe writing\n");
                exit(EXIT_FAILURE);
            }
            int ch;
            int get_pid;
            int nbChar;

            wait(NULL);

            read(mypipe[0], &get_pid, sizeof(int));
            printf("PID: %d\n", get_pid);
            read(mypipe[0], &nbChar, sizeof(int));
            printf("NBCHAR: %d\n", nbChar);

            for (int i = 0; i < nbChar; i++) {
                read(mypipe[0], &ch, sizeof(char));
                printf("LETTER: %c \n", ch);
            }

            if (close(mypipe[0]) < 0) {
                perror("[FATHER] Error closing mypipe reading\n");
                exit(EXIT_FAILURE);
            }
            break;
    }

    return 0;
}

