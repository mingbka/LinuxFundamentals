#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    if(strcmp(argv[1], "1")==0){
        printf("Run command <ls> after 2s\n");
        sleep(2);
        execl("/usr/bin/ls", "ls", NULL);
    }
    else if(strcmp(argv[1], "2")==0){
        printf("Run command <date> after 2s\n");
        sleep(2);
        execl("/usr/bin/date", "date", NULL);
    }
    else{
        printf("Vui long nhap lai");
    }
    return 0;
}