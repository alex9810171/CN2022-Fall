#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080   // Port for www webpage
#define BUFFER_SIZE 1024

int main(){
    // Server response message
    char resp[] = "HTTP/1.0 200 OK\r\n"             // request line
                  "Server: webserver-c\r\n"         // headers
                  "Content-type: text/html\r\n\r\n"
                  "<html>Name: Alex Hsu, Student ID: R10525073</html>\r\n";  // body

    // Create IPv4 TCP socket
    int sockfd = 0;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[!] socket() error: ");
        return(-1);
    }
    printf("[*] Socket created successfully!\n");

    // Initial server & client address & HTTP port
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_addrlen = sizeof(server_addr);
    int client_addrlen = sizeof(client_addr);
    bzero(&server_addr, server_addrlen);                    // Intialize struct bits with 0
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);                      // Convert host to network short
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);         // Convert host to network long

    // Let the socket bind to port
    if ((bind(sockfd, (struct sockaddr *)&server_addr, server_addrlen)) == -1)
    {
        perror("[!] bind() error: ");
        return(-1);
    }
    printf("[*] Socket bound to address successfully!\n");

    // Let the socket be connectable
    if ((listen(sockfd, SOMAXCONN)) == -1)
    {
        perror("[!] listen() error: ");
        return(-1);
    }
    printf("[*] Server listening for connections...\n");

    while (1)
    {
        // Get client connection
        int newsockfd = 0;
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&server_addr,
                                (socklen_t *)&server_addrlen)) == -1)
        {
            perror("[!] accept() error: ");
            continue;
        }

        // Get client address
        if ((getsockname(newsockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen)) == -1)
        {
            perror("[!] getsockname() error: ");
            continue;
        }
        
        // Read from socket
        char buffer[BUFFER_SIZE] = {};
        if (read(newsockfd, buffer, BUFFER_SIZE) == -1)
        {
            perror("[!] read() error: ");
            continue;
        }

        // Read request from buffer
        char method[BUFFER_SIZE] = {};
        char uri[BUFFER_SIZE] = {};
        char version[BUFFER_SIZE] = {};
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, version, uri);
        
        // Write to socket
        if ((write(newsockfd, resp, strlen(resp))) == -1)
        {
            perror("[!] write() error: ");
            continue;
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
