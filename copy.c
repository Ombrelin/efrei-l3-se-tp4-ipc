#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int readFileFd = open(argv[1], O_RDONLY);

    if(readFileFd < 0){
        perror("Error reading file\n");
        exit(EXIT_FAILURE);
    }

    struct stat st;

    if (stat(argv[1], &st) < 0) {
        perror("Error getting file status\n");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(st.st_size);

    if(buffer < 0){
        perror("Error allocating buffer\n");
        exit(EXIT_FAILURE);
    }

    read(readFileFd, buffer, st.st_size);

    int writtenFileFd = open(argv[2], O_WRONLY | O_CREAT);

    if(writtenFileFd < 0){
        perror("Error creating file\n");
        exit(EXIT_FAILURE);
    }

    write(writtenFileFd, buffer, st.st_size);

    free(buffer);

    close(readFileFd);
    close(writtenFileFd);

    return 0;
}