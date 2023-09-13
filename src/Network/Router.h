#pragma once
#include <map>
#include <vector>
#include <memory>
#include "Connection.h"
#include <string>
#include <atomic>

#include "NetType.h"

#define TIMEOUT_SECOND 120
#define TIMEOUT_CHECK_FREQUENT 30

struct ClientLifeTime
{
    int64_t client_id;
    int16_t life_time = TIMEOUT_SECOND;
};
class Router final
{
public:
    Router();
    ~Router();
    void run();
    void quit();
private:
    void handleRead(std::string& addr, unsigned short port, std::string& data);
    void handleHead(RouterHead head, std::string& addr, unsigned short port, std::string& buffer);

    void HandleHeadServer(RouterHead &head, std::string& addr, unsigned short port, std::string& buffer);
    void HandleHeadClient(RouterHead &head, std::string& addr, unsigned short port, std::string& buffer);
    void HandleHeadNone(RouterHead &head, std::string& addr, unsigned short port, std::string& buffer);
    Connection connection;
    
    std::map<int64_t,std::shared_ptr<EndPoint>> endPoint_list;
    

    int64_t check_time = TIMEOUT_CHECK_FREQUENT;
     
    int64_t server_id = -1;
    
    int16_t server_life_time = 120;
    
    std::list<int64_t> client_id_list;

    std::map<int64_t,int16_t> client_life_time;
    
    std::atomic<int64_t>  g_net_id = 100;

    bool stop = false;
};
