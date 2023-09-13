
#include <string.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <cstdio>
#include <cstring>


#define IP_ADDR "127.0.0.1"
#define PORT 8811
#define BUFFER_SIZE 1024

// 注意evutil_socket_t不仅仅可以接受socketfd，本质上是一个fd，即其他的文件描述符也是可以接受的。
void udpconn_cb(evutil_socket_t sockfd, short event, void* arg) {
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	struct sockaddr_in cliaddr;
	socklen_t socklen = sizeof(sockaddr);
	int size = recvfrom(sockfd, buffer, BUFFER_SIZE,0,(struct  sockaddr*)&cliaddr,&socklen);
	printf("%s \n",buffer);
	int r =  sendto(sockfd,buffer,size,0,(struct  sockaddr*)&cliaddr,socklen);
	std::cout << r<<std::endl;
}

int hello_main() {
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	struct event_base* evbase = nullptr;
	evbase                    = event_base_new();

	struct sockaddr_in serv_addr ;
	serv_addr.sin_family          = AF_INET;
	serv_addr.sin_port            = htons(PORT);
	serv_addr.sin_addr.s_addr     = inet_addr(IP_ADDR);

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

	struct event* ev = nullptr;
	ev = event_new(evbase, sockfd, EV_READ|EV_PERSIST, udpconn_cb, nullptr);

	event_base_set(evbase, ev);
	event_add(ev, nullptr);
	event_base_loop(evbase, EVLOOP_NO_EXIT_ON_EMPTY);
	event_base_dispatch(evbase);
    closesocket(sockfd);
	return 0;
}