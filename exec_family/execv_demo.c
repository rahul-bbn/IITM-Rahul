#include <stdio.h>
#include <unistd.h>

int main()
{
    char *args[] = {"ls", "-a", "-l", NULL};

    printf("Before execv()\n");

    execv("/bin/ls", args);

    perror("execv failed");
    return 0;
}