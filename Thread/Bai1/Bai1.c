#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t thread1;
pthread_t thread2;

void *thread1_function(){
    printf("Thread %ld is running\n", pthread_self());
    return NULL;
}

void *thread2_function(){
    printf("Thread %ld is running\n", pthread_self());
    return NULL;
}

int main(){
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
