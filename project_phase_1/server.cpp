#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Ports below 1024 are considered "privileged", above and including 1024 is "free to use" by anyone.
#define PORT 1024
#define MAX_MESSAGE_LENGTH 100
#define MAX_NUMBER_CLIENT 20

int main(){
    // create IPv4 TCP socket
    int sockfd = 0;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("[!] socket() error: ");
        return(-1);
    }

    // let connect port be reusable
    int opt = SO_REUSEADDR;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    // initial local address & HTTP port
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    bzero(&server_address,sizeof(server_address));              // intialize struct bits with 0
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);                      // convert host to network short
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);         // convert host to network long
    
    // let the socket bind to port
    int bindfd = 0;
    if((bindfd = bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address))) == -1){
        perror("[!] bind() error: ");
        return(-1);
    }

    // let the socket be connectable
    int listenfd = 0;
    if((listenfd = listen(sockfd, MAX_NUMBER_CLIENT)) == -1){
        perror("[!] listen() error: ");
        return(-1);
    }
    printf("[#] Server app activated!\n");

    // get client connection
    int connectfd = 0;
    socklen_t addrlen;
    if((connectfd = accept(sockfd, (struct sockaddr *)&client_address, &addrlen)) == -1){
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
    printf("[#] You got a connection from client IP:%s, PORT:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

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
        printf("[#] Received string: %s\n", receive_buffer);

        // send message from server
        strcpy(send_buffer, receive_buffer);
        printf("[>] Send string: %s\n", send_buffer);
        if((send_number = send(connectfd, send_buffer, MAX_MESSAGE_LENGTH, 0)) == -1){
            perror("[!] send() error: ");
            return(-1);
        }

        if(strcmp(receive_buffer, "quit") == 0){
            printf("[#] Server exit!\n");
            close(connectfd);
            break;
        }
    }

    // close connect and listen and exit
    close(connectfd);
    close(listenfd);
    return 0;
}