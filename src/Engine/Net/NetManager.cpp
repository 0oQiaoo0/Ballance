#include "NetManager.h"

#include "../GameEngine.h"
#include "../../Game/Entities/NetPlayerBall.h"
#include "../../Game/Entities/Obstacle.h"
#include "../Framework/Components/TransformComponent.h"
NetManager* NetManager::_instance = nullptr;
static uint32_t g_net_role_player_id = 1;
static uint32_t g_net_role_scene_id = 1;
#include "../Framework/NetGameObject.h"
#include "../Framework/Components/MeshRendererComponent.h"
NetManager* NetManager::GetInstance()
{
    if(_instance==nullptr)
    {
        _instance = new NetManager();
    }
    return _instance;
}
void OnNetActorJoin(UpdateActorInfo* update_actor_info_)
{
    Scene* scene =  globalContext.mGameEngine.get()->mScene.get();
    NetGameObject* obj  =nullptr;
    switch (update_actor_info_->actor_info.actorType)
    {
    case BAll_Type:
        {
            obj =  scene->AddGameObject<NetPlayerBall>();
            break;
        }
    default:
        {
            obj =  scene->AddGameObject<Obstacle>();
        }
    }
    if(obj!=nullptr)
    {
        obj->SetNetDriver(update_actor_info_);
        obj->AddToNet(NetRole::Other);
     
    }
   
}
void NetManager::HostGame()
{
    server_ =std::make_shared<Server>();
    UpdateActorInfo* update_actor_info2=nullptr;
    
    server_->OnNewNetActorJoin(OnNetActorJoin);
    std::thread([=]()
      {
          while(true)
          {
              if(update_actor_info2!=nullptr)
              {
                  PxVec3 vec3;
                 memcpy(&vec3,update_actor_info2->actor_info.transfer_data,sizeof(vec3));
                 std::cout<<"x: "<<vec3.x<<"y: "<<vec3.y<<"z: "<<vec3.z<<std::endl;
              }
             
              std::this_thread::sleep_for(std::chrono::milliseconds(400));
          }
      }).detach();
    server_->Start();
    IsServer = true;
    StartSendMsg();
}

void NetManager::JoinGame()
{
    client_ = std::make_shared<Client>();
    UpdateActorInfo* update_actor_info2=nullptr;
    
    client_->OnNewNetActorJoin(OnNetActorJoin);
    client_->JoinGame();
    server_net_id = client_->clientid;
    IsServer = false;
    StartSendMsg();
}

void NetManager::SendMsg()
{
    if(server_!=nullptr)server_->SendServerMessage();
    if(client_!=nullptr)client_->SendClientMessage();
}

void NetManager::Quit()
{
    if(server_!=nullptr)server_->Quit();
     if(client_!=nullptr)client_->Quit();
    send_thread.join();
}

bool NetManager::HasServer()
{
    return server_!=nullptr;
}

bool NetManager::IsConnect()
{
    return server_!=nullptr||client_!=nullptr;
}

UpdateActorInfo* NetManager::AddNetGameObject(NetGameObject* game_object, uint16_t ClassTypeID)
{
    if(client_==nullptr&&server_==nullptr)
    {
        std::cout<<"net no init"<<std::endl;
        return nullptr;
    }
    while(server_net_id==-1)
    {
        
        if(server_!=nullptr)server_net_id = server_->serverid;
        if(client_!=nullptr)client_net_id = client_->clientid;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    TransformComponent* transform_ = game_object->GetComponent<TransformComponent>();
    if(transform_==nullptr)
    {
        std::cout << "no transfrom can not add to net "<<std::endl;
        return nullptr;
    }
    PxVec3 vec3 =  transform_->GetPosition();
    PxVec4 quat =  transform_->GetQuat();
    ActorInfo actor_info;
    if(game_object->Net_Role==NetRole::Owner)
    {
        if(IsServer)game_object->Net_actor_id = server_net_id*64+g_net_role_player_id++;
        else game_object->Net_actor_id = client_net_id*64+g_net_role_player_id++;
    }
    else
    {
        game_object->Net_actor_id =g_net_role_scene_id++;
    }
    actor_info.netrole = game_object->Net_Role;
    actor_info.actorId = game_object->Net_actor_id;
    actor_info.actorType = ClassTypeID;
    char* index = actor_info.transfer_data;
    memcpy(index,&vec3,sizeof(vec3));
    index+=sizeof(vec3);
    memcpy(index,&quat,sizeof(quat));
    if(IsServer)
    {
        return server_->AddActor(game_object->Net_actor_id,actor_info);
    }
    else
    {
        return client_->AddActor(game_object->Net_actor_id,actor_info);
    }
}

void NetManager::StartSendMsg()
{
    stop = false;
    send_thread = std::thread([this]()
    {
        while(!stop)
        {
           
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });
}
