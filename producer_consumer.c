//
// Created by Marion on 25/05/2020.
//

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main (int argc, char** argv) {

    int mypipe[2];

    if (pipe(mypipe) < 0) {
        perror("Error opening pipe\n");
        exit(255);
    }

    int pid = fork();

    switch (pid) {
        case -1:
            perror("Error forking\n");
            exit(255);
        case 0:
            close(mypipe[0]);
            int number = 6;
            int getPid = getpid();
            write(mypipe[1], &getPid, sizeof(int));
            write(mypipe[1], &number, sizeof(int));
            char* string = "coucou";

            for(int i = 0; i<strlen(string); i++) {
                write(mypipe[1], &(string[i]), sizeof(char));
            }
            wait(NULL);
            close(mypipe[1]);
            break;
        default:
            close(mypipe[1]);
            int ch;
            int get_pid;
            int nbChar;

            read(mypipe[0], &get_pid, sizeof(int));
            printf("%d\n", get_pid);
            read(mypipe[0], &nbChar, sizeof(int));
            printf("%d\n", nbChar);

            for (int i = 0; i < nbChar; i++) {
                read(mypipe[0], &ch, sizeof(char));
                printf("%c \n", ch);
            }

            close(mypipe[0]);
            break;
    }

    return 0;
}

