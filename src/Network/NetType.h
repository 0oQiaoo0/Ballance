#pragma once
#include <string>

#define HEAD_SERVER 1
#define HEAD_CLIENT 2
#define HEAD_LISTENING_SERVER 3
#define HEAD_NONE   4

#define ACTION_REQUEST   0
#define ACTION_RESPONSE   1
#define ACTION_ACK   2
#define ACTION_OTHER  3

#define NET_ROLE_OWNER 0
#define NET_ROLE_OTHER 1
#define NET_ROLE_NONE 2
#define ANY_ADDR "0.0.0.0"

#define ROUTER_PORT 8811
#define ROUTER_ADDR "222.201.190.153"

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 0

#define CLIENT_ADDR "127.0.0.1"
#define CLIENT_PORT 0
#define MY_DEBUG 0
#define BOARDCAST_IP  "255.255.255.255"
struct RouterHead
{
    char seperate = '$';
    char seperate2 = '$';
    int8_t head_type = -1;
    int8_t action = -1;
    int16_t padding = -1;
    int64_t net_id;
};
struct EndPoint
{
    EndPoint(uint32_t net_id, const std::string& addr, unsigned short port)
        : net_id(net_id),
          addr(addr),
          port(port)
    {
    };
    uint32_t net_id;
    std::string addr;
    unsigned short port;
};
struct ActorInfo
{
    bool IsDifferentWith(ActorInfo& other);
    uint8_t netrole = -1;
    uint8_t padding2 = -1;
    uint16_t actorType = -1;
    uint32_t actorId = 0;
    char transfer_data[64];
    std::string GetBuffer();
};

inline bool ActorInfo::IsDifferentWith(ActorInfo& other)
{
    
    int res = memcmp(other.transfer_data,this->transfer_data,sizeof(transfer_data));
    return res!=0;
}

inline std::string ActorInfo::GetBuffer()
{
    std::string res(reinterpret_cast<char*>(&*this),sizeof(ActorInfo));
    return res;
}
struct UpdateActorInfo
{
    UpdateActorInfo()
    {
        IsDirty = false;
    }
    UpdateActorInfo(bool is_dirty,  const ActorInfo& actor_info)
        : IsDirty(is_dirty),
          actor_info(actor_info)
    {
    }

    bool IsDirty = false;
    ActorInfo actor_info;
    
};
