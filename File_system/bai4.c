#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int main() {
    const char *filename = "bai4.txt"; 
    const char *data = "Hello, this is bai4\n"; 

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening");
        return 1;
    }

    if (write(fd, data, strlen(data)) == -1) {
        perror("Error writing");
        close(fd);
        return 1;
    }

    close(fd); 

    struct stat file_status;
    if (stat(filename, &file_status) == -1) {
        perror("Error getting file information");
        return 1;
    }

    printf("Thông tin file:\n");
    printf("Tên file: %s\n", filename);
    printf("Kích thước: %ld bytes\n", file_status.st_size);  
    printf("Loại file: %o\n", file_status.st_mode);
    printf("Thời gian chỉnh sửa lần cuối: %s", ctime(&file_status.st_mtime));

    return 0;
}