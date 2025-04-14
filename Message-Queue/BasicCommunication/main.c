#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MQ_MODE (S_IRUSR | S_IWUSR ) 
#define QUEUE_NAME "/myqueue"
#define MAX_SIZE 20

char *msg = "Hello con";

int main(){
    pid_t pid;
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0644, &attr);
    if (mq == -1) {  
        printf("mq_open() error\n");  
        return 1;  
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        exit(1);
    }
    else if(pid > 0){
        if (mq_send(mq, msg, strlen(msg) + 1, 0) == -1) {
            perror("mq_send");
            mq_close(mq);
            mq_unlink(QUEUE_NAME);
            exit(1);
        }
        wait(NULL);

        mq_close(mq);
        mq_unlink(QUEUE_NAME);
    }
    else {
        char buffer[MAX_SIZE];
        unsigned int prio;

        // sleep(1); 
        if (mq_receive(mq, buffer, MAX_SIZE, &prio) == -1) {
            perror("mq_receive");
            exit(1);
        }

        printf("Da nhan duoc: '%s'\n", buffer);

        mq_close(mq); 
        exit(0);
    }
}