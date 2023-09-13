#include "FixedPointScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

void FixedPointScript::Start()
{
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
}

void FixedPointScript::AddFixedPoint(PxTransform transform)
{
    transformComponent->SetTransform(transform);
    rigidbodyComponent->SetDynamic(fixedPointGeometry, transformComponent->GetWorldTransform(),
        pStaticFriction, pDynamicFriction, pRestitution,
        pMass, pLinearDamping, pAngularDamping,
        pLinearVelocity, pAngularVelocity);

    rigidbodyComponent->SetSolverIterationCounts(16, 16);
}
