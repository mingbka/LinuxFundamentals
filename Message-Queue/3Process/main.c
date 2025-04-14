#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mqueue.h>
#include <ctype.h>
#include <sys/wait.h>

#define MAX_SIZE 30
#define MQ_NAME "/myqueue"

char *msg = "xin chao tat ca cac con";
pid_t pid1, pid2;
mqd_t mq;
char buffer[MAX_SIZE];
struct mq_attr attr;

int main(){
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(MQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0644, &attr);
    if (mq == -1) {  
        perror("mq_open");  
        return 1;  
    }

    pid1 = fork();
    if (pid1 < 0){
        perror("fork");
        mq_close(mq);
        mq_unlink(MQ_NAME);
        exit(1);
    }
    else if (pid1 == 0){
        // Nhan du lieu tu tien trinh cha
        if (mq_receive(mq, buffer, MAX_SIZE, 0) == -1) {
            perror("mq_receive pid1");
            exit(1);
        }
        printf("Tin nhan nhan duoc o tien trinh con 1: '%s'\n", buffer);

        // Doi tu chu thuong sang chu hoa
        for (int i=0; i<strlen(buffer); i++){
            buffer[i] = toupper(buffer[i]);
        }

        // Gui lai message queue
        if (mq_send(mq, buffer, MAX_SIZE, 0) == -1) {
            perror("mq_send pid1");
            mq_close(mq);
            mq_unlink(MQ_NAME);
            exit(1);
        }
    }
    else if(pid1 > 0){
        if (mq_send(mq, msg, MAX_SIZE, 0) == -1) {
            perror("mq_send dad");
            mq_close(mq);
            mq_unlink(MQ_NAME);
            exit(1);
        }

        // Doi tien trinh con 1 xong
        wait(NULL);

        pid2 = fork();
        if (pid2 < 0){
            perror("fork");
            exit(1);
        }
        else if (pid2 == 0){
            if (mq_receive(mq, buffer, MAX_SIZE, 0) == -1){
                perror("mq_receive pid2");
                exit(1);
            }

            printf("Tin nhan nhan duoc o tien trinh con 2: '%s'\n", buffer);
        }

        mq_close(mq);
        mq_unlink(MQ_NAME);
    }
}
