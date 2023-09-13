#include "GameProp.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../Scripts/GamePropScript.h"

void GameProp::Start()
{
	GameObject::Start();

	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();
	AddComponent<GamePropScript>();
}
