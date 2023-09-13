#include "PlayerBall.h"

#include "../../Engine/core/ThreadPool.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"


#include "../Scripts/PlayerBallScript.h"
#include "../../Engine/UI/imgui/imgui.h"
void PlayerBall::Start()
{
	NetGameObject::Start();
	
	
	ClassType = BAll_Type ;

	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();

	AddComponent<PlayerBallScript>();
}

void PlayerBall::ShowDetailUI()
{
    GameObject::ShowDetailUI();
}

void PlayerBall::ShowGameUI()
{
	GetComponent<PlayerBallScript>()->ShowUI();
}


