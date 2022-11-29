 
//#include "../shared/utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXCHAR 1024
#define PORT 8080
#define IP "127.0.0.1"

int main() {
 
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[MAXCHAR];
    int n;
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-] Socket error");
        exit(1);
    }
 
    printf("[+] TCP server socket created.\n");
 
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr(IP);
 
    n = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (n < 0) {
        printf("[-] Connection error...\n");
        exit(1);
    }
 
    printf("Connected to the server.\n");
 
    for (;;) {
 
        bzero(buffer, MAXCHAR);
        recv(sock, buffer, sizeof(buffer), 0);
        printf("\nServer: %s\n", buffer);
 
        bzero(buffer, MAXCHAR);
        printf("Client: ");
        fgets(buffer, MAXCHAR, stdin);
        send(sock, buffer, strlen(buffer), 0);
 
        if (strncmp(buffer, "close", 5) == 0 || strncmp(buffer, "4", 1) == 0) {
            close(sock);
            printf("Disconnected from the server.\n");
            exit(1);
        }
 
    }
 
 
    return 0;
}
 
