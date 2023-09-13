#include "Door.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../Scripts/DoorScript.h"

void Door::Start()
{
	GameObject::Start();

	AddComponent<DoorScript>();
}