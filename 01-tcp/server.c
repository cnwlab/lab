#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main(){
    int server_fd,new_socket;
    struct sockaddr_in address;
    int addr_len=sizeof(address);
    char buffer[1024]={0};
    char *message="hello from server";


    server_fd=socket(AF_INET,SOCK_STREAM,0);
    if (server_fd <0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr= INADDR_ANY;
    address.sin_port=htons(PORT);

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("listening");

    if(listen(server_fd,3)<0){
        perror("listening failed");
        exit(EXIT_FAILURE);
    }

    new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addr_len);
    if (new_socket<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    read(new_socket,buffer,1024);
    printf("client: %s\n",buffer);

    send(new_socket,message,strlen(message),0);
    printf("message sent to client");

    close(new_socket);
    close(server_fd);
}
