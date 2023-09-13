#include "Hammer.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../Scripts/HammerScript.h"

void Hammer::Start()
{
	GameObject::Start();

	AddComponent<HammerScript>();
}