#pragma once
#include "../../Engine/Framework/Component.h"

class TransformComponent;
class RigidbodyComponent;
class MeshRendererComponent;
class ColumnScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;
    MeshRendererComponent* meshRendererComponent;
    PxPhysics* gPhys = nullptr;;
    PxCooking* cooking = nullptr;
    std::vector<physx::PxVec3> vertices;
    /*****************************Default Settings*****************************/
    const int numSegments = 20;//圆柱的侧面顶点数量
    float angleIncrement = PxTwoPi / numSegments;


    PxReal columnStaticFriction = 0.8f;
    PxReal columnDynamicFriction = 0.8f;
    PxReal columnRestitution = 0.2f;

    PxReal columnMass = 0.03f;
    PxReal columnLinearDamping = 1.0f;
    PxReal columnAngularDamping = 1.0f;

    PxVec3 columnLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 columnAngularVelocity = { 0.0f,0.0f,0.0f };
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void Update(float DeltaTime) override;
    void AddDeafultColumn(float, float, PxTransform);
};