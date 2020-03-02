#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<pthread.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
using namespace std;
#define PORT 6666
#define BACKLOG 5
#define MAX_DATA 100
#define MAX_MEM 1000

void *unit_server(void *arg){
    int socketid = *((int *)arg);
    char buf[MAX_DATA];
    while(1){
        // cout<<socketid<<endl;
        recv(socketid,buf,MAX_DATA,0);
        send(socketid,buf,MAX_DATA,0);
        if(strcmp(buf, "quit") == 0){
            close(socketid);
            return 0;
        }
    }

}

int main(){
    int sockfd;
    int new_fd[MAX_MEM];
    pthread_t client[MAX_MEM];
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
    int i = 0;
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd[i] = accept(sockfd,(struct sockaddr*)&client_addr,&sin_size)) == -1){
            printf("receive failed.");
        }
        else{
            int ret = pthread_create(&client[i],NULL,unit_server,(void*)(&new_fd[i]));
        }
        i = i + 1;
    }
    
    return 0;
}