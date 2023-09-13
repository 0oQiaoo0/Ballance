#include "CameraEntity.h"

#include "../../Engine/Framework/Components/CameraComponent.h"

void CameraEntity::Start()
{
	GameObject::Start();
	AddComponent<CameraComponent>();
}
