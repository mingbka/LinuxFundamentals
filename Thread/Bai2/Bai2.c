#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Cần sử mutex để các thread không ghi đè lẫn nhau vào biến counter khi thực thi

#define Number_of_thread 3

int counter = 0;
pthread_mutex_t mutex;
pthread_t thread[Number_of_thread];

void *thread_function(){
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 1000000; i++){
        counter++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(){
    for(int i = 0; i < Number_of_thread; i++){
        pthread_create(&thread[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < Number_of_thread; i++){
        pthread_join(thread[i], NULL);
    }
    printf("Counter: %d", counter);
    return 0;
}