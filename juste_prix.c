//
// Created by arsen on 25/05/2020.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

int main(int arc, char **argv) {

    time_t time;
    timelocal(&time);
    srand(time);
    int prix = ((rand() % 1000 - 100 + 1)) + 100;
    printf("%d\n",prix);
    int valuePipe[2];
    if (pipe(valuePipe) < 0) {
        perror("Error openning pipe\n");
        return -1;
    }


    int pid = fork();

    if(pid == 0){
        close(valuePipe[0]);
        int input;
        printf("Donner votre nombre : \n");
        while(1){
            scanf(" %d", &input);
            write(valuePipe[1], &input, sizeof(int));
        }
    }
    else{
        close(valuePipe[1]);

        while(1){
            int value;
            read(valuePipe[0], &value, sizeof(int));

            if(value == prix){
                printf("Vous avez gagné !\n");
                close(valuePipe[0]);
                kill(pid, SIGKILL);
                break;
            }
            else {
                printf("Raté, essaie encore !\n");
                if(value < prix){
                    printf("C'est plus ! \n");
                }
                else {
                   printf("C'est moins !\n");
                }
            }
            sleep(1);
        }
    }

    return 0;
}
