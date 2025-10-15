#include <stdio.h>
#include <unistd.h>

int main()
{
    char *args[] = {"ls", "-a", NULL};

    printf("Before execvp()\n");

    execvp("ls", args);

    perror("execvp failed");
    return 0;
}