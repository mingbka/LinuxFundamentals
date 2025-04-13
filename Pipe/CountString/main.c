#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define MSG_SIZE 30

int fd[2]; 
pid_t pid;
char msg_buffer[MSG_SIZE];
int count;

char *msg = "Xin chao con, day la bo may";

int main() {
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork(); 

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        close(fd[0]); 

        write(fd[1], msg, MSG_SIZE); 
        close(fd[1]); 
    } else {
        close(fd[1]); 

        read(fd[0], msg_buffer, MSG_SIZE); 
        count = strlen(msg_buffer);
        printf("Bo noi nhung %d ky tu\n", count);

        close(fd[0]); 
        exit(0);
    }

    return 0;
}
