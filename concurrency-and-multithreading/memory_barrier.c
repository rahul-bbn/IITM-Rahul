#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdatomic.h>

static int data = 0;
static int flag = 0;

void *writer(void *arg)
{
    __atomic_store_n(&data, 42, __ATOMIC_RELAXED);
    __atomic_store_n(&flag, 1, __ATOMIC_RELEASE);

    return NULL;
}

void *reader(void *arg)
{
    int v;

    while (__atomic_load_n(&flag, __ATOMIC_ACQUIRE) == 0)
    {
       // waiting
    }

    v = __atomic_load_n(&data, __ATOMIC_RELAXED);
    printf("reader read data = %d \n", v);

    return NULL;  
}

int main(void)
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, writer, NULL);
    pthread_create(&thread2, NULL, reader, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);   
    
    return 0;
}