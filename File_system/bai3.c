/*
Viết một chương trình có só lượng command-line arguments là 3, có dạng như sau:
$ ./example_program filename num-bytes [r/w] "Hello"
Trong đó:
	1. example_grogram: Tên file thực thi
	2. filename: Tên file
	3. num-bytes: Số byte muốn read/write
	4. [r/w]: r -> Thực hiện đọc từ  filename và in ra màn hình
		    w -> Thực hiện ghi vào filename
	5. "Hello": Nội dung bất kì muốn read/write vào filename
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    if(argc != 5){
        fprintf(stderr, "Cu phap cho chuan vao");
        return 1;
    }

    const char *filename = argv[1];
    int num_bytes = atoi(argv[2]);
    char mode = argv[3][0];
    const char *data = argv[4];
    
    if(mode == 'r'){
        int fd = open(filename, O_RDONLY);
        if(fd==-1){
            perror("Error opening");
            return 1;
        }
        char *buffer = (char *)malloc(num_bytes+1);
        ssize_t bytesRead = read(fd, buffer, num_bytes);
        if(bytesRead==-1){
            perror("Error reading");
            return 1;
        }
        else{
            buffer[bytesRead]='\0';
            printf("%s", buffer);
        }
        free(buffer);
        close(fd);
    }

    else if(mode == 'w'){
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if(fd==-1){
            perror("Error opening");
            return 1;
        }
        ssize_t bytesWrite = write(fd, data, num_bytes);
        if(bytesWrite==-1){
            perror("Error Writing");
             return 1;
        }
        else{
            printf("Write successfully");
        }
        close(fd);
    }

    else{
        fprintf(stderr, "Chi nhap r hoac w thoi");
        return 1;
    }
    
    return 0;
}