//
// Created by Marion on 25/05/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void simuler_tube_shell(char* cmd1, char* cmd2) {
    int mypipe[2];
    int pid;

    if (pipe(mypipe) < 0) {
        perror("Error initialize pipe\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    switch (pid) {
        case -1:
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        case 0:
            if(close(mypipe[1]) < 0) {
                perror("Erreur dans la fermeture du pipe niveau fils en ecriture\n");
                exit(EXIT_FAILURE);
            }

            dup2(mypipe[0], 0);

            if(close(mypipe[0]) < 0) {
                perror("Erreur dans la fermeture du pipe niveau fils en lecture\n");
                exit(EXIT_FAILURE);
            }

            char* c;

            char* args[10];
            int i = 0;
            c = strtok(cmd2, "\n");

            do {
                args[i] = (char*) malloc(strlen(c) * sizeof(char));
                args[i] = c;
                printf("%s\n", c);
                c = strtok(NULL, "\n");
                i++;
            }
            while (c != NULL);

            if (execvp(args[0], args) < 0) {
                perror("Erreur exécution deuxieme commande\n");
                exit(EXIT_FAILURE);
            }

            exit(0);
        default:
            if(close(mypipe[0]) < 0){
                perror("Erreur dans la fermeture du pipe niveau pere en lecture\n");
                exit(EXIT_FAILURE);
            }

            dup2(mypipe[1], 1);

            if(close(mypipe[1]) < 0){
                perror("Erreur dans la fermeture du pipe niveau pere en ecriture\n");
                exit(EXIT_FAILURE);
            }

            char* cmd;

            cmd = strtok(cmd1, "\n");

            if (execlp(cmd, cmd, NULL) < 0) {
                perror("Erreur exécution première commande\n");
                exit(EXIT_FAILURE);
            }
            exit(0);
    }
}

int main (int argc, char** argv) {
    char cmd1[20];
    printf("Premiere commande:\n");
    fgets(cmd1,20,stdin);

    char cmd2[20];
    printf("Seconde commande:\n");
    fgets(cmd2,20,stdin);

    simuler_tube_shell(cmd1, cmd2);
    return 0;
}

