//
// Created by Marion on 25/05/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void simuler_tube_shell(char* cmd1, char* cmd2) {
    int pid = fork();
    int mypipe[2];

    if (pipe(mypipe) < 0) {
        perror("Error initialize pipe\n");
        exit(255);
    }

    switch (pid) {
        case -1:
            perror("Error forking\n");
            exit(255);
        case 0:
            close(mypipe[1]);

//            char* res;

//            read(mypipe[0], &res, sizeof(char)  * 100);

//            char* c;
//            char* args[10];
//            int i = 0;
//            c = strtok(cmd1, " ");
//            do {
//                args[i] = (char*) malloc(strlen(c) * sizeof(char));
//                args[i] = c;
//                c = strtok(NULL, " ");
//                i++;
//            }
//            while (c != NULL);

            dup2(mypipe[0], 0);
//

            close(mypipe[0]);
            //            execv(args[0],  args+1);
//            execl(cmd2, NULL);
            exit(0);
//            break;
        default:
            close(mypipe[0]);

            dup2(mypipe[1], 1);
            close(mypipe[1]);
            execl(cmd1, NULL);
            exit(0);
            break;

    }
}

int main (int argc, char** argv) {
    simuler_tube_shell("ls", "echo");
    return 0;
}

