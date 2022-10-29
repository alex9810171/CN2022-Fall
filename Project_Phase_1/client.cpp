#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // TCP connect
}