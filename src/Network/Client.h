#pragma once
#include <map>

#include "Connection.h"
#include "NetType.h"

class Client
{
public:
    friend class NetManager;
    ~Client();
    void JoinGame();
    void SendClientMessage();
    void Quit();
    void OnNewNetActorJoin(std::function<void(UpdateActorInfo*)>);
    UpdateActorInfo *AddActor(uint32_t actorid,ActorInfo actor);
private:
    void ConnectTo();
    void ConnectToRouter();
    void HandleHeadServer(RouterHead head, const std::string& addr, unsigned short port, std::string& string);
    void handleHead(RouterHead head, std::string& addr, unsigned short port, std::string& buffer);
    int64_t clientid = -1;
    Connection connection;
    std::function<void(UpdateActorInfo*)> NetActorJoinCallback;
    bool HasClientId = false;
    std::map<uint32_t,std::shared_ptr<UpdateActorInfo> > actor_list;
    bool stop = false;
    std::thread thread_  ;
};
