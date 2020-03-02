#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#define PORT 7000
#define BACKLOG 5
#define MAX_DATA 100
int main(){
    int sockfd;
    int new_fd;
    socklen_t sin_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buf[MAX_DATA];
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        printf("socket failed:%d\n",errno);
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(server_addr.sin_zero),8);
    if (bind(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr)) < 0){
        printf("bind error.\n");
        return -1;
    }
    listen(sockfd,BACKLOG);
    
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd,(struct sockaddr*)&client_addr,&sin_size)) == -1){
            printf("receive failed.");
        }
        else{
            recv(new_fd,buf,MAX_DATA,0);
            printf("%s",buf);
            send(new_fd,buf,MAX_DATA,0);
            // send(new_fd,"fuck!",5,0);
        }
    }
    return 0;
}