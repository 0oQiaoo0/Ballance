#pragma once
#include "../../Engine/Framework/Component.h"
using namespace std;
class TransformComponent;
class RigidbodyComponent;
class DoorScript :public Component
{
private:
    TransformComponent* transformComponent;
    PxPhysics* gPhys;
    PxBoxGeometry mPlankGeometry = PxBoxGeometry(PxVec3(0.1, 2.0f, 0.5f));
    /*****************************Default Settings*****************************/
    PxJointAngularLimitPair angularLimit = PxJointAngularLimitPair(PxJointAngularLimitPair(-PxPi / 2, PxPi / 4, PxSpring(1000.0f, 10.0f)));
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void CreateDoorDefault(PxTransform transform);
};