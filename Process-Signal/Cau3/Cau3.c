#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int num){
    sleep(1);
    printf("Hi dad!\n");
}

int main(){
    pid_t pid = fork();

    if(pid<0){
        perror("Error");
        return 1;
    }
    if(pid==0){
        signal(SIGUSR1, signal_handler);
        printf("Waiting for my dad...\n");
        while(1){
            pause();
        }
    }
    else{
        sleep(3);
        printf("Hi son\n");
        kill(pid, SIGUSR1);
        sleep(1);
    }
    return 0;
}