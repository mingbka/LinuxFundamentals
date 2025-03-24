#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t dem_so_chan, dem_so_le;

#define MAX 100

int so_chan = 0;
int so_le = 0;
int mang[MAX];

void *dem_so_chan_function(){
    for(int i = 0; i < MAX; i++){
        if(mang[i] % 2 == 0){
            so_chan++;
        }
    }
    return NULL;
}

void *dem_so_le_function(){
    for(int i = 0; i < MAX; i++){
        if(mang[i] % 2 != 0){
            so_le++;
        }
    }
    return NULL;
}

int main(){
    for(int i = 0; i < MAX; i++){
        mang[i] = 1 + rand() % 100;
    }
    pthread_create(&dem_so_chan, NULL, dem_so_chan_function, NULL);
    pthread_create(&dem_so_le, NULL, dem_so_le_function, NULL);
    pthread_join(dem_so_chan, NULL);
    pthread_join(dem_so_le, NULL);
    printf("So chan: %d\n", so_chan);
    printf("So le: %d\n", so_le);
    return 0;
}

