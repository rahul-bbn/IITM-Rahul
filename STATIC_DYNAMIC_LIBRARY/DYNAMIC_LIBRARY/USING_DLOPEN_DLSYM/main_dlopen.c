#include <stdio.h>
#include <dlfcn.h>   // Required for dlopen, dlsym, dlclose

int main() {
    void *handle;
    int (*add)(int, int);       // Function pointer for add()
    int (*multiply)(int, int);  // Function pointer for multiply()
    char *error;

    // Step 1: Load the shared library at runtime
    handle = dlopen("./libmymath.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        return 1;
    }

    // Step 2: Get the address of 'add' function
    add = dlsym(handle, "add");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error finding add(): %s\n", error);
        return 1;
    }

    // Step 3: Get the address of 'multiply' function
    multiply = dlsym(handle, "multiply");

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error finding multiply(): %s\n", error);
        return 1;
    }

    // Step 4: Call the functions
    printf("Addition: %d\n", add(8, 4));
    printf("Multiplication: %d\n", multiply(8, 4));

    // Step 5: Close the library
    dlclose(handle);
    return 0;
}
