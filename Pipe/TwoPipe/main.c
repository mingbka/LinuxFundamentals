#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define MSG_SIZE 100

int fd1[2], fd2[2];
char msg_buffer[MSG_SIZE];
pid_t pid1, pid2;

char *msg1 = "Xin chao";
char *msg2 = ", day la con 1";

int main() {
    // Tạo pipe
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid1 = fork(); 

    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid1 > 0) {
        close(fd1[0]); 

        write(fd1[1], msg1, MSG_SIZE); 
        close(fd1[1]); 
    } else {
        close(fd1[1]); 
        close(fd2[0]);

        read(fd1[0], msg_buffer, MSG_SIZE); 
        printf("Tien trinh con 1 nhan: %s\n", msg_buffer);

        strcat(msg_buffer, msg2);
        write(fd2[1], msg_buffer, MSG_SIZE);

        close(fd1[0]); 
        close(fd2[1]);
        exit(0);
    }

    pid2 = fork(); 

    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0){
        close(fd2[1]);

        read(fd2[0], msg_buffer, MSG_SIZE); 
        printf("Tien trinh con 2 nhan: %s\n", msg_buffer);

        close(fd2[0]);
        exit(0);
    }

    return 0;
}
