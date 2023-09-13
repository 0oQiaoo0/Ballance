#pragma once
#include "../../Engine/Framework/Component.h"
using namespace std;
class TransformComponent;
class RigidbodyComponent;
class FenceScript :public Component
{
private:
    TransformComponent* transformComponent;
    PxPhysics* gPhys;
    
    /*****************************Default Settings*****************************/
    PxBoxGeometry mPlankGeometry = PxBoxGeometry(0.05f, 0.15f, 1.5f);
    PxBoxGeometry mPushGeometry= PxBoxGeometry(0.3f, 0.4f, 0.1f);
    PxBoxGeometry mFenceGeometry = PxBoxGeometry(0.1f, 0.6f, 0.1f);


    PxReal plankStaticFriction = 0.5f;
    PxReal plankDynamicFriction = 0.5f;
    PxReal plankRestitution = 0.2f;

    PxReal plankMass = 0.5;
    PxReal plankLinearDamping = 1.0f;
    PxReal plankAngularDamping = 10.0f;
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void CreateFenceDefault(PxTransform transform);
};