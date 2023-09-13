#pragma once
#include "../../Engine/Framework/Component.h"

class TransformComponent;
class RigidbodyComponent;
class RBallTriggerScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;

public:
    void Start() override;
    void CreateRBallTrigger(PxTransform transform);
};
