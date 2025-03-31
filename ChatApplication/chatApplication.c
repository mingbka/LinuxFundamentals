#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXDATASIZE 1024
#define LISTEN_BACKLOG 5

void print_help() {
    printf("Available commands:\n");
    printf("help - Show available commands\n");
    printf("myip - Display this machine's IP\n");
    printf("myport - Show the port this process is using\n");
    printf("send <message> - Send a message\n");
    printf("exit - Close connection and terminate\n");
}

void get_my_ip() {
    char host[256];
    struct hostent *host_entry;
    if (gethostname(host, sizeof(host)) == -1) {
        perror("gethostname");
        return;
    }
    host_entry = gethostbyname(host);
    if (host_entry == NULL) {
        perror("gethostbyname");
        return;
    }
    printf("My IP: %s\n", inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <port> (server) or %s <IP> <port> (client)\n", argv[0], argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in addr;
    char buffer[MAXDATASIZE];

    // Server mode
    if (argc == 2) { 
        int port = atoi(argv[1]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return 1;
        }
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("bind");
            return 1;
        }
        listen(sockfd, LISTEN_BACKLOG);
        printf("Server listening on port %d...\n", port);

        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("accept");
            return 1;
        }
        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        while (1) {
            memset(buffer, 0, MAXDATASIZE);
            int recv_size = recv(client_fd, buffer, MAXDATASIZE, 0);
            if (recv_size <= 0) break;
            printf("Received: %s\n", buffer);
        }
        close(client_fd);

    // Client mode
    } else { 
        const char *ip = argv[1];
        int port = atoi(argv[2]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return 1;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &addr.sin_addr);

        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("connect");
            return 1;
        }
        printf("Connected to server %s:%d\n", ip, port);

        while (1) {
            printf("chat> ");
            fgets(buffer, MAXDATASIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "help") == 0) {
                print_help();
            } else if (strcmp(buffer, "myip") == 0) {
                get_my_ip();
            } else if (strcmp(buffer, "exit") == 0) {
                break;
            } else {
                send(sockfd, buffer, strlen(buffer), 0);
            }
        }
        close(sockfd);
    }
    return 0;
}
