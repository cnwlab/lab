#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main(){
    int sock;
    struct sockaddr_in serv_addr;
    int N,matrix[10][10];

    sock=socket(AF_INET,SOCK_STREAM,0);

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(PORT);

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    printf("enter order of matrix N:");
    scanf("%d",&N);

    printf("Enter elements:\n");
    for (int i =0;i<N;i++){
        for (int j =0;j<N;j++){
            scanf("%d",&matrix[i][j]);
        }
    }
    printf("matrix:\n");
    for (int i =0;i<N;i++){
        for (int j =0;j<N;j++){
            printf("%3d",matrix[i][j]);
        }
        printf("\n");
    }

    send(sock,&N,sizeof(N),0);
    send(sock,matrix,sizeof(matrix),0);
    
    char result[50];
    recv(sock,result,sizeof(result),0);

    printf("matrix type:%s\n",result);

    close(sock);
    return 0;

}