#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREAD 4
#define NUM 1000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[NUM_THREAD];

int array[NUM];
int sum[NUM_THREAD];
int total = 0;

void *thread_function(void *arg){
    int index = *(int *)arg;
    free(arg);
    for(int i = 0; i < NUM/4; i++){
        pthread_mutex_lock(&mutex);
        sum[index] += array[i + index*NUM/4];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    for(int i = 0; i < NUM; i++){
        array[i] = 1+rand() % 10;
    }
    for(int i = 0; i < NUM_THREAD; i++){
        int* arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, thread_function, arg);
    }
    for(int i = 0; i < NUM_THREAD; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NUM_THREAD; i++){
        printf("Thread %d: %d\n", i, sum[i]);
        total += sum[i];
    }
    printf("Total: %d\n", total);
    return 0;
}

