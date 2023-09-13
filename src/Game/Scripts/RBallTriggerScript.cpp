#include "RBallTriggerScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

void RBallTriggerScript::Start()
{
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
}

void RBallTriggerScript::CreateRBallTrigger(PxTransform transform)
{
	transformComponent->SetTransform(transform);
	rigidbodyComponent->SetTrigger(PxBoxGeometry(0.5f, 0.1f, 1.0f), transformComponent->GetWorldTransform());
}
