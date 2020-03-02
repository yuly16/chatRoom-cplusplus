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

struct thread_data{
    int socketfd;
    string message;
};
void *sender(void *arg){
    thread_data socket_message = *((thread_data *)arg);
    int socketfd = socket_message.socketfd;
    string message = socket_message.message;
    for(int i=0;i<10;i++){
        send(socketfd,message.c_str(),MAX_DATA,0);
    }
    send(socketfd,"quit",MAX_DATA,0);
    return 0;
}
void *receiver(void *arg){
    thread_data socket_message = *((thread_data *)arg);
    int socketfd = socket_message.socketfd;
    string message = socket_message.message;
    // cout<<"receiver"<<socket_message.message<<endl;
    // printf("receiver:%s\n",socket_message.message.c_str());
    char buf[MAX_DATA];
    while(1){
        recv(socketfd,buf,MAX_DATA,0);
        if(strcmp(buf, "quit") == 0){
                return 0;
        }
        printf("Sent:%s; Received:%s \n",message.c_str(),buf);
    }
}

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
    struct thread_data socket_message;
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
    socket_message.socketfd = socketfd;
    socket_message.message = message;

    if (connect(socketfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in)) == -1){
        printf("connect failed.\n");
    }
    else{
        printf("connect success.\n");
        // cout<<"ini"<<socket_message.message<<endl;
        int ret_send = pthread_create(&sen,NULL,sender,(void*)(&socket_message));
        int ret_receive = pthread_create(&rec,NULL,receiver,(void*)(&socket_message));
        // send(socketfd,message.c_str(),MAX_DATA,0);
        // recv(socketfd,buf,MAX_DATA,0);
        // printf("Sent:%s; Received:%s \n",message.c_str(),buf);
    }
    pthread_join(sen,NULL); 
    pthread_join(rec,NULL); 
    // pthread_exit(NULL);
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
    for(int i=0;i<NUM_THREAD;i++){
        pthread_join(clients[i],NULL); 
    }
    return 0;
}