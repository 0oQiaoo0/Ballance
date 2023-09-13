#pragma once
#include "../../Engine/Framework/Component.h"
class TransformComponent;
class RigidbodyComponent;
class MeshRendererComponent;
class ObstacleScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;
    MeshRendererComponent* meshRendererComponent;
    /*****************************Default Settings*****************************/
    //box
    PxBoxGeometry boxGeometry = PxBoxGeometry(PxVec3(0.4f, 0.4f, 0.4f));

    PxReal boxStaticFriction = 0.5f;
    PxReal boxDynamicFriction = 0.5f;
    PxReal boxRestitution = 0.3f;

    PxReal boxMass = 1.0f;
    PxReal boxLinearDamping = 0.9f;
    PxReal boxAngularDamping = 0.9f;

    PxVec3 boxLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 boxAngularVelocity = { 0.0f,0.0f,0.0f };

    //paperBall
    PxSphereGeometry ballGeometry = PxSphereGeometry(0.4f);

    PxReal paperBallStaticFriction = 0.4f;
    PxReal paperBallDynamicFriction = 0.4f;
    PxReal paperBallRestitution = 0.4f;
    
    PxReal paperBallMass = 0.2f;
    PxReal paperBallLinearDamping = 0.2f;
    PxReal paperBallAngularDamping = 0.9f;

    PxVec3 paperBallLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 paperBallAngularVelocity = { 0.0f,0.0f,0.0f };

    //woodBall
    PxReal woodBallStaticFriction = 0.6f;
    PxReal woodBallDynamicFriction = 0.6f;
    PxReal woodBallRestitution = 0.2f;

    PxReal woodBallMass = 1.0f;
    PxReal woodBallLinearDamping = 0.55f;
    PxReal woodBallAngularDamping = 0.9f;

    PxVec3 woodBallLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 woodBallAngularVelocity = { 0.0f,0.0f,0.0f };

    //stoneBall
    PxReal stoneBallStaticFriction = 0.8f;
    PxReal stoneBallDynamicFriction = 0.8f;
    PxReal stoneBallRestitution = 0.1f;

    PxReal stoneBallMass = 20.0f;
    PxReal stoneBallLinearDamping = 0.8f;
    PxReal stoneBallAngularDamping = 0.9f;

    PxVec3 stoneBallLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 stoneBallAngularVelocity = { 0.0f,0.0f,0.0f };
    /*****************************Default Settings*****************************/
public:
	void Start() override;
    void SetBoxDefault(PxTransform transform);
    void SetPaperBallDefault(PxTransform transform);
    void SetWoodBallDefault(PxTransform transform);
    void SetWoodBall(PxTransform transform, PxReal staticFriction, PxReal dynamicFriction);
    void SetStoneBallDefault(PxTransform transform);
};