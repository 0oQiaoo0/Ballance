#pragma once
#include "../../Engine/Framework/NetGameObject.h"
class RigidbodyComponent;

class NetPlayerBall: public NetGameObject
{
public:
    PxReal force = 12.0f;
    void Start() override;
    void Update(float DeltaTime) override;
    PxVec3 LastPosition;
    RigidbodyComponent* rigidbodyComponent;
};
