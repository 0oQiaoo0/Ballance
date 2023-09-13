#include "NetPlayerBall.h"

#include "PlayerBall.h"

#include "../../Engine/core/ThreadPool.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Net/NetManager.h"
#include "../Scripts/PlayerBallScript.h"

void NetPlayerBall::Start()
{
    NetGameObject::Start();

    rigidbodyComponent =  AddComponent<RigidbodyComponent>();
    rigidbodyComponent->SetDynamicDefault(PxSphereGeometry(0.4f),PxTransform(PxVec3(1,3,3)));
    rigidbodyComponent->AddToScene();
    AddComponent<MeshRendererComponent>()->RenderDefaultShape(DefaultShape::Sphere, XMFLOAT3(1, 1, 1));
;

    //AddComponent<PhysicsObject>();
}

void NetPlayerBall::Update(float DeltaTime)
{
    NetGameObject::Update(DeltaTime);


}
