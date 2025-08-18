#include<stdio.h>

int callTheStack()
{
    int localVariable = 23; //Stack memory allocated
    char buffer[1024]; //Stack memory allocated

    return 0; //Stack memory is unallocated automatically
}

int main() {
    callTheStack();
    return 0;
}


// LIFO (Last in, First Out)
// Managed by OS(CPU)
// Faster in allocation/deallocation
// Limited memory size (~7-8MB)
// Automatic cleanup
// Static memory allocation (expected behaviour)
// Sequential access pattern (Cache)