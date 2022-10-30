#include <stdio.h>
#include <sys/socket.h>

#define PORT 80
#define MAX_MESSAGE_LENGTH 100
#define MAX_NUMBER_CLIENT 20

int main(){
    // create IPv4 TCP socket
    int sockfd = 0;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket() error: ");
        return(-1);
    }
    
    
    return 0;
}