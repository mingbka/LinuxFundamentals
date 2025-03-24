#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int count = 0;
void handle_signal(int signal) {
    count++;
    if (count < 10) {
        printf("Timer: %d seconds\n", count);
        alarm(1);
    }
    else {
        printf("Time out\n");
        exit(0);
    }
}

int main() {
    signal(SIGALRM, handle_signal);
    alarm(1);
    // Nếu không gọi lại hàm alarm, chương trình sẽ chỉ nhận signal SIGALRM 1 lần và bị treo.
    while (1) {
        pause();
    }
    return 0;
}




