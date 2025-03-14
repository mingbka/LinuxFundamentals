#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    if (pid==0){
        printf("I'm child process\n");
        printf("My pid: %d\n", getpid());
    }
    else if(pid>0){
        printf("I'm parent process\n");
        printf("My child pid: %d\n", pid);
        while(1);
    }
    else{
        printf("Error\n");
    }
    return 0;
}