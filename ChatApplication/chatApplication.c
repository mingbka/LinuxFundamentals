#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <pthread.h>

#define MAX_DATASIZE 1024
#define MAX_CONNECTION 5
#define LISTEN_BACKLOG 5

// Manage number of server that you connected
typedef struct {
    int socket;
    struct sockaddr_in addr;
} Connection;

Connection connections[MAX_CONNECTION];
int connection_count = 0;

void print_help(){
    printf("Available commands:\n");
    printf("help - Show available commands\n");
    printf("myip - Display this machine's IP\n");
    printf("myport - Show the port this process is listening on\n");
    printf("connect <IP> <port> - Connect to a server\n");
    printf("list - Show all active connections\n");
    printf("terminate <id> - Terminate a connection\n");
    printf("send <id> <message> - Send a message to a connection\n");
    printf("exit - Close all connections and terminate\n");
}

void show_list(){
    printf("Active connections:\n");
    for(int i=0; i<connection_count; i++){
        // ham inet_ntoa <=> inet_ntop cho IPv4
        printf("%d: %s %d\n", i + 1, inet_ntoa(connections[i].addr.sin_addr), ntohs(connections[i].addr.sin_port));
    }
}

void terminate_connection(int id){
    if (id < 1 || id > connection_count) {
        printf("Invalid connection ID\n");
        return;
    }
    close(connections[id - 1].socket);
    for (int i = id - 1; i < connection_count - 1; i++) {
        connections[i] = connections[i + 1];
    }
    connection_count--;
    printf("Connection %d terminated\n", id);
}

void send_message(int id, char *message){
    if (id < 1 || id > connection_count) {
        printf("Invalid connection ID\n");
        return;
    }
    if (send(connections[id - 1].socket, message, strlen(message), 0) == -1) {
        perror("send");
    } else {
        printf("Message sent to %d\n", id);
    }
}

void close_all(){
    for(int i=0; i<connection_count; i++){
        close(connections[i].socket);
    }
    printf("Exiting...\n");
}

void *handle_accept(void *arg){
    int server_socket = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    while (1) {
        int new_socket_fd = accept(server_socket, (struct sockaddr*)&client_addr, &len);
        if (new_socket_fd == -1) {
            perror("Error accepting");
            continue;
        }

        printf("New connection accepted from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc!=2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number. Must be between 1 and 65535.\n");
        exit(EXIT_FAILURE);
    }

    // Handle command
    char command[50];
    int sockfd;
    struct sockaddr_in server_addr;
    fd_set read_fds;
    char buffer[MAX_DATASIZE];

    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Create server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket with ip & port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen from client
    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t accept_thread;
    pthread_create(&accept_thread, NULL, handle_accept, (void *)&sockfd);
    pthread_detach(accept_thread);

    printf("Starting chat application on port %d...\n", port);

    while(1){
        printf("chat> ");
        fgets(command, sizeof(command), stdin);
        if(strncmp(command, "help", 4)==0){
            print_help();
        } else if(strcmp(command, "myip")==0){

        } else if(strncmp(command, "myport", 6)==0){
            printf("Listening on port: %d\n", port);
        } 
        
        // Handle "connect" command
        else if(strncmp(command, "connect", 7)==0){
            char ip[30];
            int port;
            if (sscanf(command, "connect %s %d", ip, &port) != 2) {
                printf("Usage: connect <IP> <port>\n");
                continue;
            }
            int new_sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (new_sockfd == -1) {
                perror("socket");
                continue;
            }

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            // Assign ip address to server_addr.sin_addr
            inet_pton(AF_INET, ip, &server_addr.sin_addr); 

            if (connect(new_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
                perror("connect");
                close(new_sockfd);
                continue;
            }

            connections[connection_count].socket = new_sockfd;
            connections[connection_count].addr = server_addr;
            connection_count++;

            printf("Connected to %s:%d\n", ip, port);
        } 

        else if(strncmp(command, "list", 4)==0){
            show_list();
        }

        else if(strncmp(command, "terminate", 9)==0){
            int id;
            if (sscanf(command, "terminate %d", &id)==1){
                terminate_connection(id);
            } else{
                printf("Usage: terminate <connection id>\n");
            }
            
        }

        // Handle "send" command
        else if (strncmp(command, "send", 4) == 0) {
            int id;
            char message[MAX_DATASIZE];
            if (sscanf(command, "send %d %[^\n]", &id, message) == 2) {
                send_message(id, message);
            } else {
                printf("Usage: send <id> <message>\n");
            }
        }
        
        else if(strcmp(command, "exit")==0){
            close_all();
            break;
        }
        
        else{
            printf("Invalid command. Type 'help' to see available commands\n");
        }
    }
    return 0;
}