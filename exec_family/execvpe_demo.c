#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>

int main() {
    char *args[] = {"ls", "-l", NULL};
    char *envp[] = {"MYVAR=HelloWorld", NULL};
    printf("Using execvpe:\n");
    execvpe("ls", args, envp);
    perror("execvpe failed");
    return 0;
}