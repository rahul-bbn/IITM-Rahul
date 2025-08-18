#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void memory_leak() {
    char *buffer = malloc(1024);
    strcpy(buffer, "This is an intentional leak");
}

void use_after_free()
{
    char *buffer = malloc(1024);
    strcpy(buffer, "Hey from the malloc");
    free(buffer);

    printf("The buffer is %s \n", buffer);
}

void buffer_overflow()
{
    char *buffer = malloc(10);

    strcpy(buffer, "Intentional long string, which is too long for the buffer");
    free(buffer);
}

void double_free()
{
    char *buffer = malloc(1024);
    strcpy(buffer, "Hey from the malloc");
    free(buffer);

    free(buffer);
}

int main()
{
    printf("Executing code ...");

    memory_leak();
    use_after_free();
    buffer_overflow();
    double_free();

    return 0;
}