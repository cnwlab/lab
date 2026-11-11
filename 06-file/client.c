
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;
    struct sockaddr_in server;
    char filename[100], buffer[2048];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter filename: ");
    scanf("%99s", filename);

    sendto(sockfd, filename, strlen(filename) + 1, 0,
           (struct sockaddr *)&server, sizeof(server));

    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

    printf("\nServer Response:\n%s\n", buffer);

    close(sockfd);
    return 0;
}
