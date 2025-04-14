#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MQ_MODE (S_IRUSR | S_IWUSR ) 
#define QUEUE_NAME_1 "/queue1"
#define QUEUE_NAME_2 "/queue2"
#define MAX_SIZE 20

char *msg = "Hello con";
int count;

int main(){
    pid_t pid;
    mqd_t mq1, mq2;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq1 = mq_open(QUEUE_NAME_1, O_RDWR | O_CREAT | O_NONBLOCK, 0644, &attr);
    if (mq1 == -1) {  
        printf("mq_open() mq1 error\n");  
        return 1;  
    }

    mq2 = mq_open(QUEUE_NAME_2, O_RDWR | O_CREAT | O_NONBLOCK, 0644, &attr);
        if (mq2 == -1) {  
            printf("mq_open() mq2 error\n");  
            return 1;  
        }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        mq_close(mq1);
        mq_unlink(QUEUE_NAME_1);
        exit(1);
    }
    else if(pid > 0){
        if (mq_send(mq1, msg, strlen(msg) + 1, 0) == -1) {
            perror("mq_send mq1");
            mq_close(mq1);
            mq_unlink(QUEUE_NAME_1);
            exit(1);
        }
        wait(NULL);

        char num[MAX_SIZE];

        if (mq_receive(mq2, num, MAX_SIZE, 0) == -1) {
            perror("mq_receive mq2");
            exit(1);
        }

        printf("Tin nhan cha vua gui co %s bytes\n", num);

        mq_close(mq2);

        mq_close(mq1);
        mq_unlink(QUEUE_NAME_1);
    }
    else {
        char buffer[MAX_SIZE];
        unsigned int prio;

        sleep(1); 
        if (mq_receive(mq1, buffer, MAX_SIZE, &prio) == -1) {
            perror("mq_receive mq1");
            exit(1);
        }

        printf("Da nhan duoc tu bo: '%s'\n", buffer);
        count = strlen(buffer);
        snprintf(buffer, MAX_SIZE, "%d", count);
        
        // Gui tin nhan den tien trinh cha
        if (mq_send(mq2, buffer, MAX_SIZE, 0) == -1) {
            perror("mq_send mq2");
            mq_close(mq2);
            mq_unlink(QUEUE_NAME_2);
            exit(1);
        }

        mq_close(mq2);
        mq_unlink(QUEUE_NAME_2);

        mq_close(mq1); 
        exit(0);
    }
}