#include "PlankScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"

void PlankScript::Start()
{
    transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
    meshRendererComponent = GetComponent<MeshRendererComponent>();
}

void PlankScript::AddPlank(PxTransform transform, PxBoxGeometry newGeometry, PxReal plankStaticFriction,
    PxReal plankDynamicFriction, PxReal plankRestitution, PxReal plankMass, PxReal plankLinearDamping, PxReal plankAngularDamping, bool isStone)
{
    plankGeometry = newGeometry;
    this->plankStaticFriction = plankStaticFriction;
    this->plankDynamicFriction = plankDynamicFriction;
    this->plankRestitution = plankRestitution;
    this->plankMass = plankMass;
    this->plankLinearDamping = plankLinearDamping;
    this->plankAngularDamping = plankAngularDamping;
    this->isStone = isStone;
    AddPlankDefault(transform);
}

void PlankScript::AddPlank(PxTransform transform, PxBoxGeometry newGeometry, bool isStone)
{
    plankGeometry = newGeometry;
    this->isStone = isStone;
    AddPlankDefault(transform);
}

void PlankScript::AddPlankDefault(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(plankGeometry, transformComponent->GetWorldTransform(),
        plankStaticFriction, plankDynamicFriction, plankRestitution,
        plankMass, plankLinearDamping, plankAngularDamping,
        plankLinearVelocity, plankAngularVelocity);

    rigidbodyComponent->SetSolverIterationCounts(posIters, velIters);
    meshRendererComponent->RenderDefaultShape(DefaultShape::Box);
    if (isStone)
    {
        if (meshRendererComponent->renderItem.IsValid())
        {
            meshRendererComponent->renderItem.Material = DefaultMaterial::Tile;
            meshRendererComponent->renderItem.Update();
        }
    }
}

void PlankScript::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
    //auto i = transformComponent->GetWorldTransform();
    //std::cout << i.p.x << " " << i.p.y << " " << i.p.z << std::endl;
}

MeshRendererComponent* PlankScript::GetPlankMeshComponent()
{
    return this->meshRendererComponent;
}
