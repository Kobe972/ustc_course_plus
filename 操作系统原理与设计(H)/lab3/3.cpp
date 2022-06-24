#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <list>

using namespace std;

list<int> clients;

void broadcast(int srcfd);
void addfd(int epollfd, int fd);
void send_seg(int fd,char* buffer); //send segment by segment,delimited by '\n'

int main(int argc, char **argv) {
    int port = atoi(argv[1]);
    int fd;
    int num_clients=0;
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
    int epfd=epoll_create(1000);
    if(epfd<0)
    {
        perror("epfd");
        return 1;
    }
    addfd(epfd,fd);
    struct epoll_event events[1000];
    while(1)
    {
        int num_events=epoll_wait(epfd,events,1000,-1);
        if(num_events<0)
        {
            perror("epoll");
            return 1;
        }
        for(int i=0;i<num_events;i++)
        {
            int activefd=events[i].data.fd;
            if(activefd==fd&&num_clients<32)
            {
                int clientfd=accept(fd, NULL, NULL);
                addfd(epfd,clientfd);
                clients.push_back(clientfd);
		num_clients++;
            }
            else
            {
                broadcast(activefd);
            }
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
        int sent=0;
        while(sent!=strlen(to_send))
        {
            sent+=send(fd, to_send+sent, strlen(to_send)-sent, 0);
        }
        _to_send=strtok(NULL,"\n");
    }
}
void broadcast(int srcfd) {
    char buffer[1024];
    ssize_t len;
    len = recv(srcfd, buffer, 1000, 0);
    if(len==0)
    {
        close(srcfd);
        clients.remove(srcfd);
        return;
    }
    buffer[len]=0;
    for(list<int>::iterator iter=clients.begin();iter!=clients.end();iter++)
    {
        if(*iter!=srcfd)
        {
            send_seg(*iter, buffer);
        }
    }
}
void addfd(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFD,0));
}
