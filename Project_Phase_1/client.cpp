#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    // create IPv4 TCP socket, if fail -> return -1
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("Error: ");
        return(-1);
    }

    // connect to HTTP default port 80
    struct sockaddr_in address;
    bzero(&address,sizeof(address));                    // intialize struct bits with 0
    address.sin_family = AF_INET;
    address.sin_port = htons(80);                       // convert host to network short
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd,(struct sockaddr *)&address,sizeof(address));

    return 0;
}