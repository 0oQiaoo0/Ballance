#pragma once
#include <queue>
#include <string>
#include<functional>
#include <iostream>
#include <event2/util.h>
#include "../Engine/core/SyncQueue.h"


struct Buffer_info
{
    std::string addr;
    unsigned short port;
    std::string data;
};
class Connection final
{
public:
    Connection(unsigned int maxReadBuffer = 1024,
        unsigned int maxWriteBuffer = 1024,
        unsigned int maxReadThread = 3):recv_buffer(maxReadBuffer),send_buffer(maxWriteBuffer),numThread(maxReadThread){};
    friend void read_cb(evutil_socket_t , short , void*);
    friend void write_cb(evutil_socket_t , short , void*);
    
    bool set_recv_handler( std::function<  void (std::string& , unsigned short ,std::string& )> handle);
    void send(std::string addr , unsigned short port,std::string& data);
    int connect(unsigned short port,const char* address);
    //int listen(unsigned short port,const char* address = addr_any_t);
    ~Connection();
    void close();
private:
    void Recv_inThread();
    void Send_inThread();
    struct event* ev_write = nullptr;
    int numThread = 3;
    struct event_base* evbase =nullptr;
    int connect_internal(unsigned short port,const char* address);
    bool IsRunning = false;
    bool ShouldClose = false;
    std::function<  void (std::string& , unsigned short ,std::string& )> handle_ = nullptr;
    std::list<std::shared_ptr<std::thread>> mThreadGroup;
    SyncQueue<Buffer_info> recv_buffer;
    SyncQueue<Buffer_info> send_buffer;
    
};
