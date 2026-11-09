
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 9000
#define MAX 10

int main()
{
    int server, client, clients[MAX] = {0};
    struct sockaddr_in addr;
    fd_set readfds;
    char buffer[1025];
    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    // Server address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    // Bind and listen
    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);
    printf("Server started...\n");
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(server, &readfds);

        int maxfd = server;

        // Add all clients to set
        for(int i = 0; i < MAX; i++)
        {
            if(clients[i] > 0)
            {
                FD_SET(clients[i], &readfds);

                if(clients[i] > maxfd)
                    maxfd = clients[i];
            }
        }
        // Monitor sockets
        select(maxfd + 1, &readfds, NULL, NULL, NULL);
        // Accept new client
        if(FD_ISSET(server, &readfds))
        {
            client = accept(server, NULL, NULL);
            for(int i = 0; i < MAX; i++)
            {
                if(clients[i] == 0)
                {
                    clients[i] = client;
                    printf("Client %d connected\n", i + 1);
                    break;
                }
            }
        }
        // Check existing clients
        for(int i = 0; i < MAX; i++)
        {
            int sd = clients[i];
            if(sd > 0 && FD_ISSET(sd, &readfds))
            {
                int n = recv(sd, buffer, sizeof(buffer) - 1, 0);
                if(n <= 0)
                {
                    close(sd);
                    clients[i] = 0;
                    printf("Client %d disconnected\n", i + 1);
                }
                else
                {
                    buffer[n] = '\0';
                    char message[1100];
                    sprintf(message, "Client %d: %s", i + 1, buffer);
                    printf("%s", message);
                    // Broadcast to other clients
                    for(int j = 0; j < MAX; j++)
                    {
                        if(clients[j] > 0 && clients[j] != sd)
                            send(clients[j], message, strlen(message), 0);
                    }
                }
            }
        }
    }

    close(server);
    return 0;
}
