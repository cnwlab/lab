#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12345


int main(){
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr;
    socklen_t len;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(PORT);
    
    printf("enter sentence:");
    fgets(buffer,sizeof(buffer),stdin);

    len=sizeof(server_addr); 

    sendto(sockfd,buffer,strlen(buffer)+1,0,
            (struct sockaddr*)&server_addr,len);

    recvfrom(sockfd,buffer,sizeof(buffer),0,
            (struct sockaddr*)&server_addr,&len);

    printf("translated sentece:%s",buffer);

    

    close(sockfd);
    return 0;

}