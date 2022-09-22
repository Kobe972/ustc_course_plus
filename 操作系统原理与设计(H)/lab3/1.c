#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

struct Pipe {
    int fd_send;
    int fd_recv;
};
void send_seg(int fd,char* buffer); //send segment by segment,delimited by '\n'
void *handle_chat(void *data) {
    struct Pipe *pipe = (struct Pipe *)data;
    char buffer[1024];
    ssize_t len;
    while ((len = recv(pipe->fd_send, buffer, 1000, 0)) > 0) {
        buffer[len]=0;
        send_seg(pipe->fd_recv, buffer);
    }
    return NULL;
}

int main(int argc, char **argv) {
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
    int fd1 = accept(fd, NULL, NULL);
    int fd2 = accept(fd, NULL, NULL);
    if (fd1 == -1 || fd2 == -1) {
        perror("accept");
        return 1;
    }
    pthread_t thread1, thread2;
    struct Pipe pipe1;
    struct Pipe pipe2;
    pipe1.fd_send = fd1;
    pipe1.fd_recv = fd2;
    pipe2.fd_send = fd2;
    pipe2.fd_recv = fd1;
    pthread_create(&thread1, NULL, handle_chat, (void *)&pipe1);
    pthread_create(&thread2, NULL, handle_chat, (void *)&pipe2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
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
        int sent=0;
        while(sent!=strlen(to_send))
        {
            sent+=send(fd, to_send+sent, strlen(to_send)-sent, 0);
        }
        _to_send=strtok(NULL,"\n");
    }
}
