#pragma once
#include "Connection.h"
#include "NetType.h"
#include "map"


class Server
{
public:
    friend class NetManager;
    ~Server();
    void Start();
    void SendServerMessage();
    void Quit();
    void OnNewNetActorJoin(std::function<void(UpdateActorInfo*)>);
    UpdateActorInfo* AddActor(uint32_t actorid,ActorInfo actor);
private:
    void HandleHeadListenServer(RouterHead& head,  std::string& addr, unsigned short port, std::string& buffer);
    void handleHead(RouterHead head, std::string& addr, unsigned short port, std::string& buffer);
    void HandleHeadClient(RouterHead head, std::string& addr, unsigned short port, std::string& buffer);
    void ConnectToRouter();
    bool stop = false;
    std::function<void(UpdateActorInfo*)> NetActorJoinCallback;
    int64_t serverid = -1;
    Connection connection;
    bool HasServerId = false;
    std::thread thread_ ;
    std::thread thread_connent_to_router ;
    std::map<uint32_t,std::unique_ptr<UpdateActorInfo>> actor_list;
};
