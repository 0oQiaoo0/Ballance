#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Router.h"
#include <string.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <cstdio>
#include <cstring>



#define IP_ADDR "127.0.0.1"
#define PORT 8814
#define BUFFER_SIZE 1024

// 注意evutil_socket_t不仅仅可以接受socketfd，本质上是一个fd，即其他的文件描述符也是可以接受的。
void udpconn_cb(evutil_socket_t sockfd, short event, void* arg) {
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    recv(sockfd, buffer, BUFFER_SIZE,0);
    printf("%s \n",buffer);
}

int main(int argc, char** argv) {
  Router r;
r.run();
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif
    struct event_base* evbase2 = nullptr;
    evbase2                    = event_base_new();

    struct sockaddr_in serv_addr ;
    serv_addr.sin_family          = AF_INET;
    serv_addr.sin_port            = htons(8811);
    serv_addr.sin_addr.s_addr     = inet_addr("127.0.0.1");

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    struct event* ev = nullptr;
    ev = event_new(evbase2, sockfd, EV_READ , udpconn_cb, nullptr);

    event_base_set(evbase2, ev);
    event_add(ev, nullptr);
    event_base_dispatch(evbase2);
    
    closesocket(sockfd);
    return 0;
}