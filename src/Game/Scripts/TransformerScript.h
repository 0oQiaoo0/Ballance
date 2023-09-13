#pragma once
#include "../../Engine/Framework/Component.h"
#include "../Utils.h"
class TransformComponent;
class RigidbodyComponent;
class TransformerScript : public Component
{
private:
	TransformComponent* transformComponent;
	RigidbodyComponent* rigidbodyComponent;
	PxBoxGeometry transformerGeometry = PxBoxGeometry(PxVec3(0.4, 0.4, 0.4));
public:
	Utils::BallType transformerType;
	void Start() override;
	void SetTransformer(Utils::BallType transformerType, PxTransform transform);
};