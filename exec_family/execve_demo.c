#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = {"env", NULL};
    char *envp[] = {"USER=rahul", "PATH=/bin/bash", NULL};

    printf("Before execve()\n");
    execve("/usr/bin/env", args, envp);
    perror("execve failed");
    return 0;
}
