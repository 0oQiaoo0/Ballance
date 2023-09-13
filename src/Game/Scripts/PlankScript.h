#pragma once
#include "../../Engine/Framework/Component.h"

class TransformComponent;
class RigidbodyComponent;
class MeshRendererComponent;
class PlankScript : public Component
{
private:
    TransformComponent* transformComponent;
	RigidbodyComponent* rigidbodyComponent;
    MeshRendererComponent* meshRendererComponent;

    /*****************************Default Settings*****************************/
    PxBoxGeometry plankGeometry = PxBoxGeometry(PxVec3(1.0f, 0.3f, 0.4f));

    PxReal plankStaticFriction = 0.8f;
    PxReal plankDynamicFriction = 0.8f;
    PxReal plankRestitution = 0.2f;

    PxReal plankMass = 0.03f;
    PxReal plankLinearDamping = 1.0f;
    PxReal plankAngularDamping = 1.0f;

    PxVec3 plankLinearVelocity = { 0.0f,0.0f,0.0f };
    PxVec3 plankAngularVelocity = { 0.0f,0.0f,0.0f };

    PxU32 posIters = 16;
    PxU32 velIters = 16;

    bool isStone = false;
    /*****************************Default Settings*****************************/
public:
	void Start() override;
    void AddPlank(PxTransform, PxBoxGeometry, PxReal, PxReal, PxReal, PxReal, PxReal, PxReal, bool);
    void AddPlank(PxTransform, PxBoxGeometry, bool);
    void AddPlankDefault(PxTransform transform);
    void Update(float DeltaTime) override;
    MeshRendererComponent* GetPlankMeshComponent();
};

