#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    int status, rv;

    pid = fork();
    if(pid<0){
        printf("fork fail\n");
    }

    else if(pid==0){
        printf("I'm the child process, my PID: %d\n", getpid());
        printf("Exitting in 5s...\n");
        // while(1);
        sleep(5);
        exit(3);
    }
    else{
        rv=wait(&status);
        if(rv<0){
            printf("Can't wait for you, son");
        }

        printf("Waiting for my son...\n");
        
        if(WIFEXITED(status)){
            printf("Child process exit normally with status: %d\n", WEXITSTATUS(status));
        }
        else{
            printf("My son got killed abnormally!\n");
        }
    }
    return 0;
}