#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define NUM_SIGNAL 3

int signal_count = 0;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("SIGINT Received \n");;
    }
    signal_count++;
    if (signal_count == NUM_SIGNAL) {
        printf("Exit program\n");
        exit(0);
    }
}

int main() {
    if (signal(SIGINT, handle_signal) == SIG_ERR) {
        printf("Error: Unable to catch SIGINT\n");
        return 1;
    }

    while (1) {
        printf("Running... Press Ctrl+C 3 times to exit.\n");
        sleep(3);
    }

    return 0;
}

// khi bỏ qua SIGINT, chương trình sẽ bị lặp vô hạn dù có ấn CTRL C