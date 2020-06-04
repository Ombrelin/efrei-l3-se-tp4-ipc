//
// Created by Marion on 01/06/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void n_tube_shell(char** cmds, int nbCmd) {
    int pid;
    int descriptor[2];
    int descriptor2[2];

    if (pipe(descriptor) < 0) {
        perror("Error initialize pipe\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbCmd; i++) {
        pid = fork();

        switch (pid) {
            case -1:
                perror("Error forking\n");
                exit(EXIT_FAILURE);
            case 0:
                if (close(descriptor[0]) < 0) {
                    perror("Error closure pipe lecture au fils\n");
                    exit(EXIT_FAILURE);
                }
                if (close(descriptor2[1]) < 0) {
                    perror("Error closure pipe2 ecriture au fils\n");
                    exit(EXIT_FAILURE);
                }

                dup2(descriptor[1], 1);
                dup2(descriptor2[0], 0);

                if (close(descriptor[0]) < 0) {
                    perror("Error closure pipe lecture au pere\n");
                    exit(EXIT_FAILURE);
                }
                exit(0);
            default:
                if (close(descriptor[1]) < 0) {
                    perror("Error closure pipe ecriture au pere\n");
                    exit(EXIT_FAILURE);
                }

                dup2(descriptor[0], 0);

                if (close(descriptor[0]) < 0) {
                    perror("Error closure pipe lecture au pere\n");
                    exit(EXIT_FAILURE);
                }

                system(cmds[i]);
                exit(0);
        }
    }


    for (int i = 0; i < nbCmd; i++) {
        if (close(tabDf[i][0]) < 0) {
            perror("Error closure pipe lecture\n");
            exit(EXIT_FAILURE);
        }

        if (close(tabDf[i][1]) < 0) {
            perror("Error close pipe ecriture\n");
            exit(EXIT_FAILURE);
        }
    }

}

int main(int argc, char** argv) {

    return 0;
}