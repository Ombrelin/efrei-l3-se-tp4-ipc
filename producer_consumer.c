//
// Created by Marion on 25/05/2020.
//

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {

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

            int writePid = write(mypipe[1], &getPid, sizeof(int));
            int writeNumber = write(mypipe[1], &number, sizeof(int));

            if (writePid < 0) {
                perror("[SON] Error writing pipe\n");
                exit(EXIT_FAILURE);
            }

            if (writeNumber < 0) {
                perror("[SON] Error writing pipe\n");
                exit(EXIT_FAILURE);
            }

            char *string = argv[1];

            for (int i = 0; i < strlen(string); i++) {
                int writeString = write(mypipe[1], &(string[i]), sizeof(char));
                if (writeString < 0){
                    perror("[SON] Error writing pipe\n");
                    exit(EXIT_FAILURE);
                }
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

            int readPid = read(mypipe[0], &get_pid, sizeof(int));
            if (readPid < 0){
                perror("[SON] Error writing pipe\n");
                exit(EXIT_FAILURE);
            }
            printf("PID: %d\n", get_pid);
            int readNbChar = read(mypipe[0], &nbChar, sizeof(int));
            if (readNbChar < 0){
                perror("[SON] Error writing pipe\n");
                exit(EXIT_FAILURE);
            }
            printf("NBCHAR: %d\n", nbChar);

            for (int i = 0; i < nbChar; i++) {
                int readChar = read(mypipe[0], &ch, sizeof(char));
                if (readChar < 0){
                    perror("[SON] Error writing pipe\n");
                    exit(EXIT_FAILURE);
                }
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

