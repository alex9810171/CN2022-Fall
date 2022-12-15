#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1025               // Port for www webpage (Ports below 1024 are considered "privileged")
#define MAX_MESSAGE_LENGTH 100
#define BUFFER_SIZE 1024

int main(){
    // Server response message
    char resp[] = "HTTP/1.0 200 OK\r\n"             // Request line
                  "Server: webserver-c\r\n"         // Headers
                  "Content-type: text/html\r\n\r\n"
                  "<html>R10525073</html>\r\n";     // Body

    // Create IPv4 TCP socket
    int sockfd = 0;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("[!] socket() error: ");
        return(-1);
    }
    printf("[*] Socket created successfully!\n");

    // Let connect port be reusable
    int opt = SO_REUSEADDR;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Initial server & client address & HTTP port
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_addrlen = sizeof(server_addr);
    int client_addrlen = sizeof(client_addr);
    bzero(&server_addr, sizeof(server_addr));               // Intialize struct bits with 0
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);                     // Convert host to network short
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);        // Convert host to network long
    
    // Let the socket bind to port
    if ((bind(sockfd, (struct sockaddr *)&server_addr, server_addrlen)) == -1)
    {
        perror("[!] bind() error: ");
        return(-1);
    }
    printf("[*] Socket bound to address successfully!\n");

    // Let the socket be connectable
    int listenfd = 0;
    if((listenfd = listen(sockfd, SOMAXCONN)) == -1){
        perror("[!] listen() error: ");
        return(-1);
    }
    printf("[*] Server listening for connections...\n");

    // Get client connection
    int connectfd = 0;
    socklen_t addrlen;
    if((connectfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) == -1){
        perror("[!] accept() error: ");
        return(-1);
    }

    // send welcome message
    char welcome_message[] = "[#] Welcome! You can input any message or 'quit' to exit";
    if((send(connectfd, welcome_message, sizeof(welcome_message), 0))==-1){
        perror("[!] send welcome message fail: ");
        return(-1);
    }

    // notification of connection
    printf("[#] You got a connection from client IP:%s, PORT:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while(1){
        char receive_buffer[MAX_MESSAGE_LENGTH] = {};
        char send_buffer[MAX_MESSAGE_LENGTH] = {};
        ssize_t send_number = 0;
        ssize_t receive_number = 0;

        // receive message from client
        if((receive_number = recv(connectfd, receive_buffer, MAX_MESSAGE_LENGTH, 0)) == -1){
            perror("[!] recv() error: ");
            return(-1);
        }
        printf("[*] Received string: %s\n", receive_buffer);

        // send message from server
        strcpy(send_buffer, receive_buffer);
        printf("[>] Send string: %s\n", send_buffer);
        if((send_number = send(connectfd, send_buffer, MAX_MESSAGE_LENGTH, 0)) == -1){
            perror("[!] send() error: ");
            return(-1);
        }

        if(strcmp(receive_buffer, "quit") == 0){
            printf("[*] Server exit!\n");
            close(connectfd);
            break;
        }
    }

    // close connect and listen and exit
    close(connectfd);
    close(listenfd);
    return 0;
}