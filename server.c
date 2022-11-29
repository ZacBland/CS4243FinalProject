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

//#include "../shared/utils.h"
 
char readArray[720][7 * 250];
 
int main() {
 
    int server_sock, client_sock;
    struct sockaddr_in serveraddr = {0}, client_addr = {0};
    socklen_t addr_size;
    int n;
    char buffer[MAXCHAR];
    pid_t clientPID;
 
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
 
    if (server_sock < 0) {
        perror("[-] Socket error");
        exit(1);
    }
 
    printf("[+] TCP server socket created.\n");
 
    memset(&serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = PORT;
    serveraddr.sin_addr.s_addr = inet_addr(IP);
 
    n  = bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (n < 0) {
        perror("[-] Bind error");
        exit(1);
    }
 
    printf("[+} Bind to the port number: %d\n", PORT);
 
    if (listen(server_sock, 5) ==0) {
        printf("[+] Listening...\n");
    } else {
        printf("[-] Bind error...\n");
    }
 
    // TODO: Read and store options.txt
    //struct Option* options = readOptions("../options.txt", 2);
 
    for (;;) {
 
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr_in*)&client_addr, &addr_size);
        if (client_sock < 0) {
            exit(1);
        }
 
        printf("[+] Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
 
 
        if ((clientPID = fork()) == 0) {
            close(server_sock);
 
                //TODO: Implement main menu here
        }
    }
 
    close(client_sock);
 
    return 0;
}
 
 
