#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
    const char *filename = "hello.txt";
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT);
    char buf[12] = "hello world";

    if(fd==-1){
        perror("Error opening");
        return 1;
    }

    lseek(fd, 0, SEEK_SET);
           
    write(fd, "hello world!\n", strlen(buf));

    close(fd);
}