//
// Created by arsen on 25/05/2020.
//

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int readFileFd = open("test.txt", O_RDONLY);

    struct stat st;
    stat("test.txt",&st);

    char* buffer = malloc(st.st_size);
    read(readFileFd, buffer, st.st_size);

    int writtenFileFd = open("target.txt", O_WRONLY | O_CREAT);
    write(writtenFileFd, buffer, st.st_size);


    if(buffer < 0){
        perror("Error allocating buffer");
    }

    if(readFileFd < 0){
        perror("Error reading file");
    }

    if(writtenFileFd < 0){
        perror("Error creating file");
    }

    free(buffer);
    close(readFileFd);
    close(writtenFileFd);

}