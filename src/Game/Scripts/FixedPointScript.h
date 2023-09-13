#pragma once
#include "../../Engine/Framework/Component.h"

class TransformComponent;
class RigidbodyComponent;
class FixedPointScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;

    PxSphereGeometry fixedPointGeometry = PxSphereGeometry(0.01f);

    //²ÎÊý
    PxReal pStaticFriction = 0.8f;
    PxReal pDynamicFriction = 0.8f;
    PxReal pRestitution = 0.2f;

    PxReal pMass = 0.03f;
    PxReal pLinearDamping = 1.0f;
    PxReal pAngularDamping = 1.0f;

    PxVec3 pLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 pAngularVelocity = { 0.0f,0.0f,0.0f };

public:
    void Start() override;
    void AddFixedPoint(PxTransform);
};