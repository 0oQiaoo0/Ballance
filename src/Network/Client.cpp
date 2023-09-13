#include "Client.h"

#include <DirectXMath.h>
#include <memory>

#include "NetType.h"

Client::~Client()
{
    thread_.join();
}

void Client::JoinGame()
{
    stop = false;
    ConnectTo();
    
}

void Client::SendClientMessage()
{
    RouterHead head;
    head.net_id = clientid;
    head.head_type =HEAD_CLIENT;
    head.action = ACTION_OTHER;
    std::string buffer(reinterpret_cast<char*>(&head),sizeof(head));
    bool send = false;
    for(auto& actor:actor_list)
    {
        if(actor.second->IsDirty)
        {
            buffer+=actor.second->actor_info.GetBuffer();
            send = true;
        }
    }
    if(send)
    {
        connection.send(ROUTER_ADDR,ROUTER_PORT,buffer);
    }
}

void Client::Quit()
{
    stop = true;
    connection.close();
    thread_.join();
}

void Client::OnNewNetActorJoin(std::function<void(UpdateActorInfo*)> callback)
{

    NetActorJoinCallback = callback;
}

UpdateActorInfo *Client::AddActor(uint32_t actorid, ActorInfo actor)
{

    if(!HasClientId)return nullptr;
    if(actor_list.count(actorid)>0)
    {
        return  actor_list[actorid].get();
    }
    actor_list[actorid] =std::make_shared<UpdateActorInfo>(true,actor);
    SendClientMessage();
    return actor_list[actorid].get();
    
}

void Client::ConnectTo()
{
    connection.set_recv_handler([&](std::string& addr, unsigned short port, std::string& buffer)
   {
      // std::cout<<"recv from addr: "<<addr<<"port:"<<port<<" size:"<<buffer.size()<<std::endl;
       RouterHead head;
       memcpy(&head,buffer.data(),sizeof(head));
       handleHead(head,addr,port,buffer);
       //memcpy(&head,buffer.data(),sizeof(head));
       //handleHead(head,addr,port,buffer);

   });
    thread_ =  std::thread([&]()
    {
        connection.connect(CLIENT_PORT,  ANY_ADDR);
    });
    int maxTry = 99;
    int i = 0;
    while(!HasClientId&&i++<maxTry&&!stop)
    {
        std::cout<<"trying connect to router ... round "<<i<<std::endl;
        RouterHead h;
        h.net_id = 123;
        h.head_type = HEAD_CLIENT;
        h.action = ACTION_REQUEST;
        char buf[1024];
        memcpy(buf,&h,sizeof(h));
        std::string buffer(buf,sizeof(h));
        //buffer.resize(sizeof(h));
        connection.send(ROUTER_ADDR ,ROUTER_PORT,buffer);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }


}



void Client::HandleHeadServer(RouterHead head, const std::string& addr, unsigned short port, std::string& buffer)
{
    const char * index = buffer.data();
    index+=sizeof(head);
    int i = sizeof(head);
    //std::cout<<"recv server buffer size:"<<buffer.size()<<std::endl;
    while(i<buffer.size())
    {
        ActorInfo actor;
        memcpy(&actor,index,sizeof(actor));
        if(actor.netrole == NET_ROLE_OWNER||actor.netrole == NET_ROLE_NONE)
        {
            if(actor_list.count(actor.actorId)>0)
            {
                //std::cout<<"client actor update "<<actor.actorId<<std::endl;
                if(actor_list[actor.actorId]->actor_info.IsDifferentWith(actor))
                {
                    actor_list[actor.actorId]->actor_info = actor;
                    actor_list[actor.actorId]->IsDirty =true;
                }
            }
            else
            {
                std::cout<<"client add actor, actor id "<<actor.actorId<<std::endl;
                actor_list[actor.actorId] = std::make_shared<UpdateActorInfo>(true,actor);
                if(NetActorJoinCallback!=nullptr)
                {
                    NetActorJoinCallback(actor_list[actor.actorId].get());
                }
            }
        }

       
        i+=sizeof(ActorInfo);
        index+=sizeof(ActorInfo);
    }
    //BufferMeta meta
}

void Client::handleHead(RouterHead head, std::string& addr, unsigned short port, std::string& buffer)
{
    switch (head.head_type)
    {
    case HEAD_CLIENT:
        {
            switch (head.action)
            {
            case ACTION_RESPONSE:
                {
                    std::cout<<"client get id"<<head.net_id<<std::endl;
                    clientid = head.net_id;
                    head.action = ACTION_ACK;
                    char * a =const_cast<char *> (buffer.data());
                    memcpy(a,&head,sizeof(head));
                    connection.send(addr,port,buffer);
                    HasClientId = true;
                    break;
                }
            default:
                {
                    break;
                }
            }
            break;
        }
    case HEAD_SERVER:
        {
            HandleHeadServer(head,addr,port,buffer);
            break;
        }

    default:
        {
            std::string rec = "invalid buffer:"+buffer;
            connection.send(addr,port,rec);
            break;
        }
    }
}
