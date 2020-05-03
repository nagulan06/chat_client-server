#include <sys/socket.h> /* struct sockaddr */
#include <sys/types.h>  /* struct sockaddr */
#include <netinet/in.h>
#include <netinet/ip.h> /* getaddrinfo */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h> /* inet_pton */
#include <unistd.h>


# define PORT 5000

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: %s <ip>\n", argv[0]);
    }
    int clientfd = 0;
    struct sockaddr_in serv_addr;
    char sendbuf[50];
    char readbuf[50];
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    // Fill up the sockaddr_in struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("inet_pton error: %s\n", strerror(errno));
        exit(0);
    }  
    
    while(1)
    {
        // client socket fd
        clientfd = socket(AF_INET, SOCK_STREAM, 0);
        if(clientfd == -1)
        {
            printf("socket call: %s\n", strerror(errno));
            exit(0);
        }
        //connect with the server
        if(connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("connect error: %s\n", strerror(errno));
            exit(0);
        }
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(readbuf, 0, sizeof(readbuf));
        // Send data
        printf("client: ");
        scanf(" %[^\n]", sendbuf);
        write(clientfd, sendbuf, sizeof(sendbuf));
        // Receive data
        printf("server: ");
        read(clientfd, readbuf, sizeof(readbuf));
        printf("%s\n", readbuf);
        close(clientfd);
    }
}