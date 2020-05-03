#include <sys/socket.h> /* struct sockaddr */
#include <sys/types.h>  /* struct sockaddr */
#include <netinet/in.h>
#include <netinet/ip.h> /* getaddrinfo */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


# define PORT 5000

int main()
{
    int listenfd = 0, connfd = 0, optval = 1;
    char sendbuf[50];
    char readbuf[50];
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    // Fill the sockaddr_in struct
    serv_addr.sin_family = AF_INET;     // IPV4 address family
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY enables the server to bind to any IP address

    // listenfd will be socket on which the server listens for connections
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        printf("socket call: %s\n", strerror(errno));
        exit(0);
    }

    // Eliminates "Address already in use" error from bind 
    int rv = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    if(rv == -1)
    {
        printf("setsockopt error: %s\n", strerror(errno));
        exit(0);
    }
    
    // Assigning a name to the socket
    rv = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(rv == -1)
    {
        printf("bind call: %s\n", strerror(errno));
        exit(0);
    }
    
    // listen on the listenfd socket
    rv = listen(listenfd, 10);
    if(rv == -1)
    {
        printf("listen call: %s\n", strerror(errno));
        exit(0);
    }
    
    while(1)
    {
        // Initialize buffer to 0.
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(readbuf, 0, sizeof(readbuf));
        // Accept connection from the client
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        
        // read data from the client and print it
        printf("%s", "client: ");
        read(connfd, readbuf, sizeof(readbuf));
        printf("%s\n", readbuf);
 
        // Send data to client
        printf("server: ");
        scanf(" %[^\n]", sendbuf);
        write(connfd, sendbuf, sizeof(sendbuf)); 

        if(close(connfd) == -1)
        {
            printf("close(connfd): %s\n", strerror(errno));
            exit(0);
        }
    }

    return 0;
}
