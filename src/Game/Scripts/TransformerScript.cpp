#include "TransformerScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

void TransformerScript::Start()
{
	Component::Start();
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
}

void TransformerScript::SetTransformer(Utils::BallType transformerType, PxTransform transform)
{
	this->transformerType = transformerType;
	transformComponent->SetTransform(transform);
	rigidbodyComponent->SetTrigger(transformerGeometry, transformComponent->GetWorldTransform());
}