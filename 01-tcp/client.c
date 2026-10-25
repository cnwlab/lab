#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main(){
    int sock=0;
    struct sockaddr_in serv_addr;
    char *message="hello from client";
    char buffer[1024]={0};


    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock<0){
        printf("socket failed");
        return -1;
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
        printf("invalid address");
        return -1;
    }

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        printf("connection failed");
        return -1;
    }

    send(sock,message,strlen(message),0);
    printf("message sent to server");

    read(sock,buffer,1024);
    printf("server:%s\n",buffer);

    close(sock);

    return 0;

}