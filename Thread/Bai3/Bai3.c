#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t producer, consumer;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t send_data = PTHREAD_COND_INITIALIZER;
pthread_cond_t receive_data = PTHREAD_COND_INITIALIZER;

int data;
int new_data = 0;

void *producer_function(void *arg){
    int loop = 10;
    while(loop -- > 0){
        pthread_mutex_lock(&mutex);
        while(new_data){
            pthread_cond_wait(&receive_data, &mutex);
        }
        data = rand() % 10;
        printf("Producer: %d\n", data);
        new_data = 1;
        pthread_cond_signal(&send_data);
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

void *consumer_function(void *arg){
    int loop = 10;
    while(loop -- > 0){
        pthread_mutex_lock(&mutex);
        while(!new_data){
            pthread_cond_wait(&send_data, &mutex);
        }
        printf("\tConsumer: %d\n", data);
        new_data = 0;
        pthread_cond_signal(&receive_data);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_create(&producer, NULL, producer_function, NULL);
    pthread_create(&consumer, NULL, consumer_function, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    return 0;
}
