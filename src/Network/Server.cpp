#include "Server.h"

#include "NetType.h"


Server::~Server()
{
    thread_.join();
}

void Server::Start()
{
    stop=false;
    connection.set_recv_handler([&](std::string& addr, unsigned short port, std::string& buffer)
    {
#if MY_DEBUG

        std::cout<<"recv from addr: "<<addr<<"port:"<<port<<"data"<<buffer<<std::endl;
#endif
        RouterHead head;
        memcpy(&head,buffer.data(),sizeof(head));
        handleHead(head,addr,port,buffer);
        //memcpy(&head,buffer.data(),sizeof(head));
        //handleHead(head,addr,port,buffer);

    });
    thread_ =  std::thread([&]()
    {
        connection.connect(SERVER_PORT,  ROUTER_ADDR);
    });
    

    thread_connent_to_router = std::thread([&]()
    {
        int maxTry = 99;
        int i2 = 1;
        while(!HasServerId&&i2++<maxTry)
        {
            std::cout<<"trying connect to router ... round "<<i2<<std::endl;
            RouterHead h;
            h.net_id = 123;
            h.head_type = HEAD_SERVER;
            h.action = ACTION_REQUEST;
            char buf[1024];
            memcpy(buf,&h,sizeof(h));
            std::string buffer(buf,sizeof(h));
            //buffer.resize(sizeof(h));
            connection.send(BOARDCAST_IP ,ROUTER_PORT,buffer);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
   
    std::cout<<"server start successfully"<<std::endl;

}

void Server::HandleHeadListenServer(RouterHead& head, std::string& addr, unsigned short port, std::string& buffer)
{
    const char * index = buffer.data();
    index+=sizeof(head);
    int i = sizeof(head);
    while(i<addr.size())
    {
        ActorInfo actor;
        memcpy(&actor,index,sizeof(actor));
        if(actor.netrole == NET_ROLE_OWNER||actor.netrole == NET_ROLE_NONE)
        {
            if(actor_list.count(actor.actorId)>0)
            {
                if(actor_list[actor.actorId]->actor_info.IsDifferentWith(actor))
                {
                    actor_list[actor.actorId]->actor_info = actor;
                    actor_list[actor.actorId]->IsDirty =true;
                }
            }
            else
            {
                actor_list[actor.actorId] = std::make_unique< UpdateActorInfo>(true,actor);
            }
        }
        i+=sizeof(head);
        index+=sizeof(head);
    }
    //BufferMeta meta
    SendServerMessage();
}

void Server::handleHead(RouterHead head, std::string& addr, unsigned short port, std::string& buffer)
{
    switch (head.head_type)
    {
    case HEAD_SERVER:
        {
            switch (head.action)
            {
            case ACTION_RESPONSE:
                {
                    std::cout<<"server get id"<<head.net_id<<std::endl;
                    serverid = head.net_id;
                    head.action = ACTION_ACK;
                    char * a =const_cast<char *> (buffer.data());
                    memcpy(a,&head,sizeof(head));
                    connection.send(addr,port,buffer);
                    HasServerId = true;
                    break;
                }
            default:
                {
                    break;
                }
            }
            break;
        }
    case HEAD_CLIENT:
        {
            HandleHeadClient(head,addr,port,buffer);
            break;
        }
    case HEAD_LISTENING_SERVER:
        {
            HandleHeadListenServer(head,addr,port,buffer);
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

void Server::HandleHeadClient(RouterHead head, std::string& addr, unsigned short port, std::string& buffer)
{
#if MY_DEBUG
    std::cout<<"server rec client msg"<<"size:"<<buffer.size()<<"data: "<<buffer<<std::endl;
#endif
    
    const char * index = buffer.data();
    index+=sizeof(head);
    int i = sizeof(head);
    while(i<buffer.size())
    {
        ActorInfo actor;
        memcpy(&actor,index,sizeof(actor));
        if(actor.netrole == NET_ROLE_OWNER)
        {
            if(actor_list.count(actor.actorId)>0)
            {
                if(actor_list[actor.actorId]->actor_info.IsDifferentWith(actor))
                {
#if MY_DEBUG
                    std::cout<<"server found dirty actor "<<std::endl;
#endif
                    
                    actor_list[actor.actorId]->actor_info = actor;
                    actor_list[actor.actorId]->IsDirty =true;
                }
            }
            else
            {
                std::cout<<"server add actor, actor id "<<actor.actorId<<std::endl;
                actor_list[actor.actorId] = std::make_unique< UpdateActorInfo>(true,actor);
                if(actor_list.size()==2)
                {
                    std::cout<<"server add actor, actor id "<<actor.actorId<<std::endl;
                }
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
    SendServerMessage();
} 

void Server::ConnectToRouter()
{

    
}

void Server::SendServerMessage()
{
#if MY_DEBUG
    std::cout<<"server trying send msg to router"<<std::endl;
#endif
    
    RouterHead head;
    head.net_id = serverid;
    head.head_type =HEAD_SERVER;
    head.action = ACTION_OTHER;
    std::string buffer(reinterpret_cast<char*>(&head),sizeof(head));
    bool send = false;
    for(auto& actor:actor_list)
    {
        if(actor.second->IsDirty)
        {
            buffer+=actor.second->actor_info.GetBuffer();
            send = true;
            //actor.second->IsDirty = false;
        }
    }
    if(send)
    {
#if MY_DEBUG
        std::cout<<"server send msg to router"<<std::endl;
#endif
        
        connection.send(ROUTER_ADDR,ROUTER_PORT,buffer);
    }
}

void Server::Quit()
{
    stop = true;
    connection.close();
    thread_.join();
    thread_connent_to_router.join();
}

void Server::OnNewNetActorJoin(std::function<void(UpdateActorInfo*)> callback)
{
    NetActorJoinCallback = callback;
}

UpdateActorInfo* Server::AddActor(uint32_t actorid, ActorInfo actor)
{
    if(!HasServerId)return nullptr;
    if(actor_list.count(actorid)>0)
    {
        return  actor_list[actorid].get();
    }
    actor_list[actorid] =std::make_unique<UpdateActorInfo>(true,actor);
    SendServerMessage();
    return actor_list[actorid].get();

}
