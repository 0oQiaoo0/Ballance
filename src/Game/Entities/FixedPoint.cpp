#include "FixedPoint.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "../Scripts/FixedPointScript.h"

void FixedPoint::Start()
{
	GameObject::Start();

	AddComponent<RigidbodyComponent>();
	AddComponent<FixedPointScript>();
}