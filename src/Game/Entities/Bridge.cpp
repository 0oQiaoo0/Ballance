#include "Bridge.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../Scripts/BridgeScript.h"

void Bridge::Start()
{
	GameObject::Start();

	AddComponent<BridgeScript>();
}