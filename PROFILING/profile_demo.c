#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
 
int main() {
    int fd = open("sample.txt", O_RDONLY | O_CREAT);
    char buf[100];
    read(fd, buf, 50);
    write(1, buf, 50);
    close(fd);

    // printf("Hello, World!\n");
    // int *arr = malloc(5 * sizeof(int));
    // free(arr);


    return 0;
}