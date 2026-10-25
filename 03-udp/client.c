#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    len=sizeof(server_addr);

    char *msg="Hello from client";
    sendto(sockfd,msg,strlen(msg)+1,0,
            (struct sockaddr*)&server_addr,len);

    recvfrom(sockfd,buffer,sizeof(buffer),0,
            (struct sockaddr*)&server_addr,&len);
    printf("server:%s\n",buffer);
    

    close(sockfd);
    return 0;
    
}