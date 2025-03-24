#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

volatile sig_atomic_t sigint_received = 0;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("SIGINT received.\n");
    } else if (signal == SIGTERM) {
        printf("SIGTERM received. Exiting...\n");
        exit(0);
    }
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    fd_set readfds;
    char buffer[256];

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL) > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    printf("You entered: %s", buffer);
                }
            }
        }
    }

    return 0;
}
// Chưa hiểu ChatGPT code này làm gì