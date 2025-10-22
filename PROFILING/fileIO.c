#include <stdio.h>
#include <stdlib.h>
 
int main() {
    FILE *fp = fopen("demo.txt", "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }
 
    for (int i = 0; i < 5; i++) {
        fprintf(fp, "This is line %d\n", i);
    }
 
    fclose(fp);
    return 0;
}