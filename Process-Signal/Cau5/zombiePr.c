#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid==0){
        printf("I'm child process\n");
        printf("My pid: %d\n", getpid());
        exit(0);
    }
    else if(pid>0){
        int status;
        printf("I'm parent process\n");
        printf("Hello zombie %d\n", pid);
        while(1);
        exit(0);
    }
    else{
        printf("Error\n");
    }
    return 0;
}