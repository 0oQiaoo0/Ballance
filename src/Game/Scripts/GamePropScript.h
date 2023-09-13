#pragma once
#include "../../Engine/Framework/Component.h"

#include "../Utils.h"

class TransformComponent;
class RigidbodyComponent;
class MeshRendererComponent;
class GamePropScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;
    MeshRendererComponent* meshRendererComponent;

    PxSphereGeometry gamePropGeometry = PxSphereGeometry(0.4f);
public:
    Utils::GamePropType gamePropType;
    //timeProp
    float extraTime = 0.0f;
    void Start() override;
    void SetGameProp(Utils::GamePropType gamePropType, PxTransform transform, float time = 0.0f);
    void SetGameProp(Utils::GamePropType gamePropType,PxVec3 scale, PxTransform transform, float time = 0.0f);
};