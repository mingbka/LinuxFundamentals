#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    int fd[2]; 
    pid_t pid;
    char write_msg[] = "Xin chao tu tien trinh cha!";
    char read_msg[100];

    // Tạo pipe
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

        write(fd[1], write_msg, strlen(write_msg) + 1); 
        close(fd[1]); 
    } else {
        close(fd[1]); 

        read(fd[0], read_msg, sizeof(read_msg)); 
        printf("Tien trinh con nhan: %s\n", read_msg);

        close(fd[0]); 
        exit(0);
    }

    return 0;
}
