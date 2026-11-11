#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char filename[100], buffer[2048];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    printf("Server Started...\n");

    while(1)
    {
        recvfrom(sockfd, filename, sizeof(filename), 0,
                 (struct sockaddr *)&client, &len);
        if(fork() == 0)
        {
            FILE *fp = fopen(filename, "r");
            sprintf(buffer, "Server PID: %d\n", getpid());
            if(fp == NULL)
                strcat(buffer, "File Not Found");
            else
            {
                char line[200];
                while(fgets(line, sizeof(line), fp))
                    strcat(buffer, line);

                fclose(fp);
            }
            sendto(sockfd, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&client, len);
            printf("Request for %s served by PID %d\n", filename, getpid());       
            exit(0);
        }
    }
    close(sockfd);
    return 0;
}
