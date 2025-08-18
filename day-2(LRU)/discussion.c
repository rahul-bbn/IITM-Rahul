//memory fragmentation
//unpredictable nature
//memory overhead

char *buffer1 = malloc(1024); //1KB
char *buffer2 = malloc(2048);

free(buffer1);

char *buffer3 = malloc(4096); //4KB

//Memory context

Application Layer
    1. Query Memory Context
    2. Transaction Memory Context
    3. Session Memory Context
        a. Buffer pool
        b. Index pool
        c. tuple pool
        d. temp pool (joins, sorting, etc)