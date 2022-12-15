#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1025               // Port for www webpage (Ports below 1024 are considered "privileged")
#define MAX_MESSAGE_LENGTH 100

int main(int argc, char *argv[]){
    // create IPv4 TCP socket
    int sockfd = 0;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("[!] socket() error: ");
        return(-1);
    }

    // initial local address & HTTP port
    struct sockaddr_in address;
    bzero(&address,sizeof(address));                    // intialize struct bits with 0
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);                     // convert host to network short
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect to server
    if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1){
        perror("[!] connect() error: ");
        return(-1);
    }
    printf("[*] Client app activated!\n");

    // receive welcome message
    char welcome_message[] = {};
    if ((recv(sockfd, welcome_message, MAX_MESSAGE_LENGTH,0)) == -1){
    	perror("[!] Receive welcome message fail: ");
    	return(-1);
    }
    welcome_message[sizeof(welcome_message)-1] = '\0';
    printf("%s\n", welcome_message);

    // send message to server
    while(1){
        char message_buffer[MAX_MESSAGE_LENGTH] = {};
        char receive_buffer[MAX_MESSAGE_LENGTH] = {};
        char welcome_buffer[MAX_MESSAGE_LENGTH] = {};
        ssize_t send_number = 0;
        ssize_t receive_number = 0;

        // user input message
        printf("[>] Input string: ");
        scanf("%s", message_buffer);
        message_buffer[strlen(message_buffer)]='\0';    // null terminator
        if((strlen(message_buffer)) > MAX_MESSAGE_LENGTH){
            printf("[!] Only supports sending fewer than 100 characters.");
            return(-1);
        }

        // send message to server
        if((send_number = send(sockfd, message_buffer, MAX_MESSAGE_LENGTH, 0)) == -1){
            perror("[!] send() error: ");
            return(-1);
        }

        // receive message from server
        if((receive_number = recv(sockfd, receive_buffer, MAX_MESSAGE_LENGTH,0)) == -1){
            perror("[!] recv() error: ");
            return(-1);
        }

        // quit client condition
        if(strcmp(receive_buffer, "quit") == 0){
            break;
        }
    }

    // close socket and exit
    close(sockfd);
    return 0;
}
