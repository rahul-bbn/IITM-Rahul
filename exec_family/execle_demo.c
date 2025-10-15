#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Before execle()\n");

    // Prepare a custom environment array. It must be NULL-terminated.
    // We purposely set a small environment with one custom var.
    char *envp[] = { "MyVar=EXECLE", "PATH=/usr/bin:/bin", NULL };

    // Call execle:
    // - first argument: path to the executable we want to run (here: /usr/bin/env)
    // - next arguments: argument list for the new program (argv[0] is typically the program name)
    // - the argument list must be terminated by NULL
    // - last parameter: envp array (custom environment)
    execle("/usr/bin/env", "env", NULL, envp);

    // If execle returns, it failed. Print a helpful error message.
    // errno is set by execle on failure.
    perror("execle failed!!");
    return 0;
}
