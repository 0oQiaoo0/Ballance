#include "ObstacleScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
void ObstacleScript::Start()
{
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
	meshRendererComponent = GetComponent<MeshRendererComponent>();
}

void ObstacleScript::SetBoxDefault(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(boxGeometry, transformComponent->GetWorldTransform(),
        boxStaticFriction, boxDynamicFriction, boxRestitution,
        boxMass, boxLinearDamping, boxAngularDamping,
        boxLinearVelocity, boxAngularVelocity);

    meshRendererComponent->RenderDefaultShape(DefaultShape::Box);
}

void ObstacleScript::SetPaperBallDefault(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(ballGeometry, transformComponent->GetWorldTransform(),
        paperBallStaticFriction, paperBallDynamicFriction, paperBallRestitution,
        paperBallMass, paperBallLinearDamping, paperBallAngularDamping,
        paperBallLinearVelocity, paperBallAngularVelocity);

    meshRendererComponent->RenderDefaultShape(DefaultShape::Sphere);
    meshRendererComponent->renderItem.Material = DefaultMaterial::Paper;
    meshRendererComponent->renderItem.Update();
}

void ObstacleScript::SetWoodBallDefault(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(ballGeometry, transformComponent->GetWorldTransform(),
        woodBallStaticFriction, woodBallDynamicFriction, woodBallRestitution,
        woodBallMass, woodBallLinearDamping, woodBallAngularDamping,
        woodBallLinearVelocity, woodBallAngularVelocity);

    meshRendererComponent->RenderDefaultShape(DefaultShape::Sphere);
    meshRendererComponent->renderItem.Material = DefaultMaterial::Wood;
    meshRendererComponent->renderItem.Update();
}

void ObstacleScript::SetStoneBallDefault(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(ballGeometry, transformComponent->GetWorldTransform(),
        stoneBallStaticFriction, stoneBallDynamicFriction, stoneBallRestitution,
        stoneBallMass, stoneBallLinearDamping, stoneBallAngularDamping,
        stoneBallLinearVelocity, stoneBallAngularVelocity);

    meshRendererComponent->RenderDefaultShape(DefaultShape::Sphere);
    meshRendererComponent->renderItem.Material = DefaultMaterial::Tile;
    meshRendererComponent->renderItem.Update();
}

void ObstacleScript::SetWoodBall(PxTransform transform, PxReal staticFriction, PxReal dynamicFriction)
{
    woodBallStaticFriction = staticFriction;
    woodBallDynamicFriction = dynamicFriction;
    SetWoodBallDefault(transform);
}
