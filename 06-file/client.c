#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 2048

int main()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t len=sizeof(server);
    char filename[100];
    char buffer[BUFFER_SIZE];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd<0)
    {
        printf("Socket Creation Failed\n");
        return 0;
    }


    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");


    printf("Enter File Name : ");

    scanf("%s",filename);

    sendto(sockfd,
           filename,
           strlen(filename)+1,
           0,
           (struct sockaddr *)&server,
           len);

    recvfrom(sockfd,
             buffer,
             sizeof(buffer),
             0,
             NULL,
             NULL);

    printf("Response Received From Server\n");

    printf("%s\n",buffer);

    close(sockfd);

    return 0;
}