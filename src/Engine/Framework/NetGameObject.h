#pragma once
#include <intsafe.h>

#include "GameObject.h"
#include "../../Network/NetType.h"
#include "Components/TransformComponent.h"

class RigidbodyComponent;

enum NetRole
{
    Owner = 0,
    Other ,
    None,
};
#define BAll_Type  133
#define OBS_Type 134
/**
 * \brief 网络物体，会自动发送位置信息和物理信息
 * 自动同步物理信息未实现
 */
class NetGameObject:public GameObject
{
public:
    uint8_t NetUpdateTime = 2;
    uint8_t CurrentUpdateTime = 2;
    TransformComponent * transform_component = nullptr;
    void Start() override;
    NetGameObject();
    ~NetGameObject();
    virtual  void SetNetDriver(UpdateActorInfo* update_actor_info);
    virtual  void AddToNet(NetRole net_role=NetRole::None);
    virtual  void Update(float DeltaTime) override;
    uint16_t ClassType = 0;
    uint32_t Net_actor_id;
    uint8_t Net_Role = 99;
    PxVec3 LastPosition;
    UpdateActorInfo* update_actor_info =nullptr;
    RigidbodyComponent* rigidBodyComponent =nullptr;
};
