#include "Plank.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../Scripts/PlankScript.h"

void Plank::Start()
{
	GameObject::Start();
	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();
	AddComponent<PlankScript>();
}