#include "Fence.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../Scripts/FenceScript.h"

void Fence::Start()
{
	GameObject::Start();

	AddComponent<FenceScript>();
}
