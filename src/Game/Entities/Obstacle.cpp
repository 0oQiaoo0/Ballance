#include "Obstacle.h"

#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../Scripts/ObstacleScript.h"

#include "../../Engine/UI/imgui/imgui.h"

void Obstacle::Start()
{
	NetGameObject::Start();

	ClassType = OBS_Type;

	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();
	AddComponent<ObstacleScript>();
}

void Obstacle::ShowDetailUI()
{
    GameObject::ShowDetailUI();
}



