#include <stdio.h>
#include <unistd.h>
 
int add(int a, int b) {
    return a + b;
}
 
int main() {
    for (int i = 0; i < 5; i++) {
        int result = add(i, i * 2);
        printf("add(%d, %d) = %d\n", i, i * 2, result);
        sleep(1);
    }
    return 0;
}