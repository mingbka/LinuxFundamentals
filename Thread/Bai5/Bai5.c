#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int data = 0;
pthread_rwlock_t rwlock;

void* reader_function(void* arg) {
    int id = *((int*)arg);
    free(arg);

    pthread_rwlock_rdlock(&rwlock);
    printf("Reader %d: read data = %d\n", id, data);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void* writer_function(void* arg) {
    int id = *((int*)arg);
    free(arg);

    pthread_rwlock_wrlock(&rwlock);
    data++;
    printf("Writer %d: incremented data to %d\n", id, data);
    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    pthread_rwlock_init(&rwlock, NULL);

    // Tạo các thread đọc
    for (int i = 0; i < NUM_READERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader_function, id);
    }

    // Tạo các thread ghi
    for (int i = 0; i < NUM_WRITERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer_function, id);
    }

    // Đợi tất cả các thread hoàn thành
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("Final data value: %d\n", data);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
