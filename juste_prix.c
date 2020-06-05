//
// Created by arsen on 25/05/2020.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int generate_random_number(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

int main(int arc, char **argv) {

    int min = 100, max = 1000;
    int input = 0, victory = 2;

    srand(time(NULL));

    int prix = generate_random_number(min, max);
    printf("*** Prix a deviner: %d ***\n", prix);

    int valuePipe[2];
    if (pipe(valuePipe) < 0) {
        perror("Error opening value pipe\n");
        exit(EXIT_FAILURE);
    }

    int victoryPipe[2];
    if (pipe(victoryPipe) < 0) {
        perror("Error opening victory pipe\n");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    switch (pid) {
        case -1:
            perror("Error while forking\n");
            exit(EXIT_FAILURE);
        case 0:
            if (close(valuePipe[0]) < 0) {
                perror("[SON] Error closing value pipe in reading\n");
                exit(EXIT_FAILURE);
            }

            if (close(victoryPipe[1]) < 0) {
                perror("[SON] Error closing victory pipe in writing\n");
                exit(EXIT_FAILURE);
            }

            while(1) {
                if (read(victoryPipe[0], &victory, sizeof(int)) < 0) {
                    perror("[SON] Erreur lecture victory pipe");
                    exit(EXIT_FAILURE);
                }

                switch (victory) {
                    case -1:
                        min = input;
                        break;
                    case 0:
                        if (close(valuePipe[1]) < 0) {
                            perror("[SON] Error closing value pipe in writing\n");
                            exit(EXIT_FAILURE);
                        }

                        if (close(victoryPipe[0]) < 0) {
                            perror("[SON] Error closing victory pipe in reading\n");
                            exit(EXIT_FAILURE);
                        }
                        exit(EXIT_SUCCESS);
                    case 1:
                        max = input;
                        break;
                    default:
                        break;
                }

                input = generate_random_number(min, max);
                printf("[SON] Est-ce %d ?\n", input);

                if (write(valuePipe[1], &input, sizeof(int)) < 0) {
                    perror("[SON] Erreur ecriture value pipe");
                    exit(EXIT_FAILURE);
                }
            }
        default:
            if (close(valuePipe[1]) < 0) {
                perror("[FATHER] Error closing value pipe in writing\n");
                exit(EXIT_FAILURE);
            }

            if (close(victoryPipe[0]) < 0) {
                perror("[FATHER] Error closing victory pipe in reading\n");
                exit(EXIT_FAILURE);
            }

            do {
                if (input > 100) {
                    if(input == prix){
                        printf("[FATHER] Vous avez gagné !\n");
                        victory = 0;
                    }
                    else {
                        if(input < prix){
                            printf("[FATHER] C'est plus ! \n");
                            victory = -1;
                        }
                        else {
                            printf("[FATHER] C'est moins !\n");
                            victory = 1;
                        }
                    }
                }

                if (write(victoryPipe[1], &victory, sizeof(int)) < 0) {
                    perror("[FATHER] Error writing victory pipe\n");
                    exit(EXIT_FAILURE);
                }

            } while (read(valuePipe[0], &input, sizeof(int)) > 0);
    }

    if (close(valuePipe[0]) == -1) {
        perror("[FATHER] Error closing value pipe in reading\n");
        exit(EXIT_FAILURE);
    }

    if (close(victoryPipe[1]) == -1) {
        perror("[FATHER] Error closing victory pipe in writing\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
