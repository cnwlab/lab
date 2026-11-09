#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sock;
    struct sockaddr_in server;
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to server!\n");

    if(fork() == 0)
    {
        // Child: Send messages
        while(fgets(buffer, sizeof(buffer), stdin))
            send(sock, buffer, strlen(buffer), 0);
    }
    else
    {
        // Parent: Receive messages
        int n;

        while((n = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
    }

    close(sock);
    return 0;
}