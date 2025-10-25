/*
Write a C program that simulates a bank account with multiple threads performing deposits and withdrawals.
Ensure correct balance updates using mutex synchronization.
Print all transactions and final balance. Save as multi_thread_bank.c.
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define THREADS 4
#define TRANSACTIONS 5

int balance=1000;
pthread_mutex_t balance_mutex=PTHREAD_MUTEX_INITIALIZER;

void deposit(int amount,int thread_id){
    pthread_mutex_lock(&balance_mutex);
    balance=balance+amount;
    printf("Thread %d: Deposited %d | New Balance: %d\n",thread_id,amount,balance);
    pthread_mutex_unlock(&balance_mutex);
}

void withdraw(int amount,int thread_id){
    pthread_mutex_lock(&balance_mutex);
    if (balance >= amount)
    {
        balance=balance-amount;
        printf("Thread %d: Withdrawn %d | New Balance: %d\n",thread_id,amount,balance);
    } else{
        printf("Thread %d: Withdrawal of %d failed (Insufficient balance)\n",thread_id,amount);
    }
    pthread_mutex_unlock(&balance_mutex);
}

void* transactions(void* arg){
    int thread_id = *((int*)arg);

    for (int i = 0; i < TRANSACTIONS; i++)
    {
        int amount = (rand() % 200) + 50; // Random amount between 50–249
        int action = rand() % 2; // 0 = deposit, 1 = withdraw

        if (action == 0)
        {
            deposit(amount,thread_id);
        }
        else{
            withdraw(amount,thread_id);
        }
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t threads[THREADS];
    int thread_ids[THREADS];

    srand(time(NULL));

    printf("Initial Balance: %d\n", balance);
    printf("Starting transactions...\n\n");

    for (int i = 0; i < THREADS; i++)
    {
        thread_ids[i]=i+1;
        pthread_create(&threads[i],NULL,transactions,&thread_ids[i]);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    printf("\nAll transactions complete.\n");
    printf("Final Balance: %d\n",balance);

    pthread_mutex_destroy(&balance_mutex);
    return 0;
}
