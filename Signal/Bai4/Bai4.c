#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int signal) {
    if (signal == SIGTSTP) {
        printf("SIGTSTP ignored\n");      
    }
}

int main() {
    signal(SIGTSTP, handle_signal);
    while (1) {
        printf("Running...\n");
        sleep(3);
    }
    return 0;
}
    
// Nếu không xử lý signal SIGTSTP, khi nhấn Ctrl+Z chương trình sẽ bị stop tạm thời chứ không bị kill.
// Chương trình có thể tiếp tục chạy khi sử dụng lệnh fg.