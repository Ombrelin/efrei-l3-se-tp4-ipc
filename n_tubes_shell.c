//
// Created by Marion on 01/06/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void n_tube_shell(char** cmds, int nbCmd) {
    int pid;
    int pipes[nbCmd][2];

    for (int i = 0; i < nbCmd; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("Error initialize pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nbCmd; i++) {
        pid = fork();

        switch (pid) {
            case -1:
                perror("Error forking\n");
                exit(EXIT_FAILURE);
            case 0:
                if (close(pipes[i][0]) < 0) {
                    perror("Error closure pipe lecture au fils\n");
                    exit(EXIT_FAILURE);
                }

                dup2(pipes[i][1], 1);
                dup2(pipes[i][0], 0);

                if (close(pipes[i][0]) < 0) {
                    perror("Error closure pipe lecture au pere\n");
                    exit(EXIT_FAILURE);
                }
                exit(0);
            default:
                if (close(pipes[i][1]) < 0) {
                    perror("Error closure pipe ecriture au pere\n");
                    exit(EXIT_FAILURE);
                }

                dup2(pipes[i][0], 0);

                if (close(pipes[i][0]) < 0) {
                    perror("Error closure pipe lecture au pere\n");
                    exit(EXIT_FAILURE);
                }

                system(cmds[i]);
                exit(0);
        }
    }

}

int main(int argc, char** argv) {

    char* cmds[2] = {"ls", "wc"};
    n_tube_shell(cmds, 2);

    return 0;
}