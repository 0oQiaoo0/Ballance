#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include "Connection.h"
#define BUFFER_SIZE 1024
#include <assert.h>
#include <string.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <cstdio>
#include <cstring>


#include <functional>
#include <event2/event.h>s


void read_cb(evutil_socket_t sockfd, short event, void* connection) {

    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(sockaddr);
    int size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct  sockaddr*)&cliaddr, &socklen);
    if(size<=0){
        std::cout<<"recv buffer fail "<<std::endl;
        return ;
    }
   
    std::string data(std::move(buffer),size);
    Buffer_info info;
    in_addr ipv4Addr;
    char c_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cliaddr.sin_addr, c_addr,sizeof(sockaddr));
    info.addr = c_addr;
    info.port = ntohs(cliaddr.sin_port) ;
    info.data = std::move(data);
    reinterpret_cast<Connection*>(connection)->recv_buffer.Put(std::move(info));

    
}
void write_cb(evutil_socket_t sockfd, short event, void* connection)
{
  
    Connection* con =  reinterpret_cast<Connection*>(connection);
    if(con->send_buffer.Empty())return;
    Buffer_info buffer_info;
    con->send_buffer.Take(buffer_info);
    if(buffer_info.data.empty())return;
    struct sockaddr_in sin;
    in_addr ipv4Addr;
    int result = inet_pton(AF_INET, buffer_info.addr.c_str(), &ipv4Addr);
    assert(result==1);
    sin.sin_family =  AF_INET;
    sin.sin_addr = ipv4Addr;
    sin.sin_port =htons(buffer_info.port) ;
    int r = sendto(sockfd, buffer_info.data.data(), buffer_info.data.size(), 0, (struct  sockaddr*)&sin, sizeof(sin));
    if(r<=0)
    {
        std::cout<<"send buffer fail  ip:"<<buffer_info.addr<<" port "<<ntohs(sin.sin_port)<<std::endl;
    }
}



bool Connection::set_recv_handler( std::function<  void (std::string& , unsigned short ,std::string& )> handle)
{
    if(handle_!=nullptr)
    {
        std::cout<<"has already set handler"<<std::endl;
        return false;
    }
    handle_ = handle;
    return true;
}

void Connection::send(std::string addr, unsigned short port, std::string& data)
{
    Buffer_info buffer_info;
    buffer_info.addr = addr;
    buffer_info.data =data;
    buffer_info.port = port;
    send_buffer.Put(std::move(buffer_info));
}

void Connection::Recv_inThread()
{
    while (!ShouldClose)
    {
        Buffer_info buffer_info;
        recv_buffer.Take(buffer_info);
        if (ShouldClose)return;
        handle_(buffer_info.addr,buffer_info.port,buffer_info.data);
    }
}
void Connection::Send_inThread()
{

}
int Connection::connect(unsigned short port, const char* address)
{
    if(handle_==nullptr)
    {
        std::cout<<"warning no recv handle set"<<std::endl;
    }
    else
    {
        for (unsigned int i = 0; i <numThread; ++i)
        {
            mThreadGroup.push_back(
                std::make_shared<std::thread>(&Connection::Recv_inThread, this));
        }
    }
    return connect_internal(port,address);
}



Connection::~Connection()
{
    close();
}

void Connection::close()
{
    ShouldClose =true;
    for (auto thread : mThreadGroup)
    {
        if (thread)thread->join();
    }
    event_base_loopexit(evbase,nullptr);
}



int Connection::connect_internal(unsigned short port, const char* address)
{
    if(IsRunning)
    {
        std::cout<<"connection is running"<<std::endl;
        return -1;
    }
if(ShouldClose)return -1;
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif
   // struct event_base* evbase2 = nullptr;
    evbase                    = event_base_new();

    struct sockaddr_in serv_addr ;
    serv_addr.sin_family          = AF_INET;
    serv_addr.sin_port            = htons(port);
    serv_addr.sin_addr.s_addr     = inet_addr(address);

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    const char enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) < 0) {
        std::cerr << "Failed to set socket option\n";
        return 1;
    }
    
    int re =  bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    
    struct event* ev = nullptr;
    
    ev = event_new(evbase, sockfd, EV_READ|EV_PERSIST , read_cb, this);
    ev_write = event_new(evbase, sockfd, EV_WRITE|EV_PERSIST , write_cb, this);
    event_base_set(evbase, ev);
   
    event_add(ev, nullptr);
    event_base_set(evbase, ev_write);
    event_add(ev_write, nullptr);
    event_base_dispatch(evbase);
    
    closesocket(sockfd);
    return 0;
}
