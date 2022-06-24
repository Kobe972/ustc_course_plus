#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <queue>
using namespace std;

int num_client = 0;
pthread_mutex_t mutex_socket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_MessageQueue = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_fds = PTHREAD_MUTEX_INITIALIZER;
int fds[32];
struct message
{
    int dst_fd;
    string to_send;
};
queue<message> MessageQueue;
void *handle_queue(void *param);
void send_seg(int fd,char* buffer); //send segment by segment,delimited by '\n'
void *handle_chat(void *data) {
    int *_data = (int *)data;
    int my_fd=*_data;
    char buffer[1024];
    ssize_t len;
    while (1) {
        len=recv(my_fd, buffer, 1000, 0);
        if(len<=0) break;
        buffer[len]=0;
        send_seg(my_fd, buffer);
    }
    for(int i=0;i<32;i++)
    {
        pthread_mutex_lock(&mutex_fds);
        if(fds[i]==my_fd) 
        {
            fds[i]=-1;
            pthread_mutex_unlock(&mutex_fds);
            break;
        }
        pthread_mutex_unlock(&mutex_fds);
    }
    return NULL;
}

int main(int argc, char **argv) {
    for(int i=0;i<32;i++) fds[i]=-1;
    int port = atoi(argv[1]);
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    socklen_t addr_len = sizeof(addr);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind");
        return 1;
    }
    if (listen(fd, 2)) {
        perror("listen");
        return 1;
    }
    pthread_t thread;
    pthread_create(&thread, NULL, handle_queue, NULL);
    while(1)
    {
        if(num_client<32)
        {
            int fd1=accept(fd,NULL,NULL);
            if(fd==-1)
            {
                perror("accept");
                return 1;
            }
            num_client++;
            for(int i=0;i<32;i++)
            {
                pthread_mutex_lock(&mutex_fds);
                if(fds[i]==-1)
                {
                    fds[i]=fd1;
                    pthread_mutex_unlock(&mutex_fds);
                    break;
                }
                pthread_mutex_unlock(&mutex_fds);
            }
            pthread_t thread;
            pthread_create(&thread, NULL, handle_chat, (void *)&fd1);
        }
    }
    return 0;
}
void send_seg(int fd,char* buffer)
{
    char* _to_send=strtok(buffer,"\n");
    char to_send[1024];
    while(_to_send!=NULL)
    {
        strcpy(to_send,"Message: ");
        strcat(to_send,_to_send);
        strcat(to_send,"\n");
        for(int i=0;i<32;i++)
        {
            if(fds[i]==-1||fds[i]==fd) continue;
            int sent=0;
            message _message;
            _message.dst_fd=fds[i];
            _message.to_send=string(to_send);
            pthread_mutex_lock(&mutex_MessageQueue);
            MessageQueue.push(_message);
            pthread_mutex_unlock(&mutex_MessageQueue);
            
        }
        _to_send=strtok(NULL,"\n");
    }
}
void* handle_queue(void* param)
{
    while(1)
    {
        pthread_mutex_lock(&mutex_MessageQueue);
        while(!MessageQueue.empty())
        {
            message _message=MessageQueue.front();
            MessageQueue.pop();
            int sent=0;
            while(sent!=strlen(_message.to_send.c_str()))
            {
                sent+=send(_message.dst_fd, _message.to_send.c_str()+sent, strlen(_message.to_send.c_str())-sent, 0);
            }
        }
        pthread_mutex_unlock(&mutex_MessageQueue);
    }
    return NULL;
}