#include "NetGameObject.h"

#include "../Net/NetManager.h"
#include "Components/RigidbodyComponent.h"


void NetGameObject::Start()
{
    GameObject::Start();
    transform_component = GetComponent<TransformComponent>();
}

NetGameObject::NetGameObject()
{
}

NetGameObject::~NetGameObject()
{
    
}

void NetGameObject::SetNetDriver(UpdateActorInfo* update_actor_info_)
{
    Net_Role =  NetRole::Other;
    update_actor_info = update_actor_info_;
    rigidBodyComponent = GetComponent<RigidbodyComponent>();
}

void NetGameObject::AddToNet(NetRole net_role)
{
    this->Net_Role  = net_role;
    if(update_actor_info!=nullptr)
    {
        Net_actor_id = update_actor_info->actor_info.actorId;
       return;
    }
    if(!NetManager::GetInstance()->IsConnect())
    {
        LOG_ERROR("has not connect to server");
        return;
    }
    uint16_t id =ClassType;
    update_actor_info = NetManager::GetInstance()->AddNetGameObject(this,id );
    if(update_actor_info!=nullptr)
    {
        Net_actor_id = update_actor_info->actor_info.actorId;
    }
   
    NetManager::GetInstance()->Actor_type_map[id] = typeid(*this).name();
}
//temp 
PxVec3 q[2];
int frame = 3;
int cur = 0;
void NetGameObject::Update(float DeltaTime)
{
    GameObject::Update(DeltaTime);
    if(update_actor_info==nullptr)
    {
        if(transform_component!=nullptr)
            LastPosition = transform_component->GetPosition();
        goto out;
    }
    if(update_actor_info==nullptr||transform_component==nullptr)goto out;
    //other ball
    if(Net_Role==NetRole::Other)
    {
        PxVec3 pos;
        PxVec4 quat;
     
        char * index = update_actor_info->actor_info.transfer_data;
        memcpy(&pos,index,sizeof(pos));
        index+=sizeof(pos);
        memcpy(&quat,index,sizeof(quat));
      
        //PxVec3 vec3N = {pos.x,0,pos.z};
        //PxVec3 LastPositionN = {LastPosition.x,0,LastPosition.z};
        PxVec3 vec3new  = pos;
        if(LastPosition!=pos)
        {
            vec3new = ((1-cur/frame)*LastPosition)+((cur/frame)*pos);
            cur = (cur+1)%frame;
            if(vec3new ==pos)
            {
                LastPosition=pos;
            }
        }
        transform_component->SetPosition(vec3new);
        std::cout<<"set pos"<<pos.y<<std::endl;
        if(rigidBodyComponent!=nullptr&&!rigidBodyComponent->isStatic)
        {
            PxVec3 liner;
            PxVec3 anglur;
            index+=sizeof(quat);
            memcpy(&liner,index,sizeof(liner));
            index+=sizeof(liner);
            memcpy(&anglur,index,sizeof(anglur));
            rigidBodyComponent->SetLinearVelocity(liner);
            rigidBodyComponent->SetAngularVelocity(anglur);
        }
        LastPosition =pos;
    }
    else
    {
        if(CurrentUpdateTime>0)
        {
            CurrentUpdateTime--;
        }
        else
        {
            CurrentUpdateTime = NetUpdateTime;
           

            bool IsServer = NetManager::GetInstance()->HasServer();
            if(Net_Role==NetRole::Owner&&Net_actor_id ==update_actor_info->actor_info.actorId||
                IsServer&&Net_Role==NetRole::None)
            {
                RigidbodyComponent* rigidBody = GetComponent<RigidbodyComponent>();
                PxVec3 vec3 = transform_component->GetPosition();
                PxVec4 vec4 = transform_component->GetQuat();
                char * index =update_actor_info->actor_info.transfer_data;
                memcpy(index,&vec3,sizeof(vec3));
                index+=sizeof(vec3);
                memcpy(index,&vec4,sizeof(vec4));
                if(rigidBody!=nullptr&& rigidBody->GetRigidActor()!=nullptr )
                {
                    auto acotr = rigidBody->GetRigidActor();
                    PxRigidDynamic* dynamic =(PxRigidDynamic*)(acotr);
                    if(dynamic)
                    {
                        PxVec3 liner =  dynamic->getLinearVelocity();
                        PxVec3 angular =  dynamic->getAngularVelocity();
                        index+=sizeof(vec4);
                        memcpy(index,&liner,sizeof(liner));
                        index+=sizeof(liner);
                        memcpy(index,&angular,sizeof(angular));
                    }
                  
                }
                update_actor_info->IsDirty = true;
                NetManager::GetInstance()->SendMsg();
            }
            else
            {
                PxVec3 vec3;
                PxVec4 vec4;
                char * index =  update_actor_info->actor_info.transfer_data;
                memcpy(&vec3,index,sizeof(vec3));
                index+=sizeof(vec3);
                memcpy(&vec4,index,sizeof(vec4));
                transform_component->SetPosition(vec3);
                transform_component->SetQuat(vec4);
            }
        }
    }

    out:
        auto a = 1;
}
