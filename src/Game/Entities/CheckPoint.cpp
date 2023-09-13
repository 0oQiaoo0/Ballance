#include "CheckPoint.h"

#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "../Scripts/CheckPointScript.h"

void CheckPoint::Start()
{
	GameObject::Start();

	AddComponent<RigidbodyComponent>();
	AddComponent<CheckPointScript>();
}

void CheckPoint::ShowDetailUI()
{
	GameObject::ShowDetailUI();
}
