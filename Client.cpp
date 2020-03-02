#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<iostream>
#include <stdlib.h>
using namespace std;
#define NUM_THREAD 16
#define SERVER_PORT 6666
#define SERVER_IP "127.0.0.1"
#define MAX_DATA 100

void *requests(void *arg){
    int socketfd;
    struct sockaddr_in server_addr;
    pthread_t sen;
    pthread_t rec;
    char buf[MAX_DATA];
    int id = *((int *)arg);
    char id_char[10];
    sprintf(id_char,"%d",id);
    string message = "TCP ";
    message += id_char;
    // printf("%s\n",message.c_str());
    printf("Client initialization.\n");
    if ((socketfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        printf("socket failed.");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bzero(&(server_addr.sin_zero),sizeof(server_addr.sin_zero));

    if (connect(socketfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in)) == -1){
        printf("connect failed.\n");
    }
    else{
        printf("connect success.\n");
        // int ret_send = pthread_create(&sen,NULL,sender,message);
        // int ret_receive = pthread_create(&rec,NULL,receiver,message);
        send(socketfd,message.c_str(),MAX_DATA,0);
        recv(socketfd,buf,MAX_DATA,0);
        printf("Sent:%s; Received:%s \n",message.c_str(),buf);
    }
    close(socketfd);
    return 0;
}


int main(){
    int ret;
    pthread_t clients[NUM_THREAD];
    int id[NUM_THREAD];
    for (int i=0;i<NUM_THREAD;i++){
        id[i] = i;
        ret = pthread_create(&clients[i],NULL,requests,(void*)(&id[i]));
    }
    pthread_exit(NULL);
    return 0;
}