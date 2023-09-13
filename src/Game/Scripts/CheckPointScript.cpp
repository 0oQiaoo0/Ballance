#include "CheckPointScript.h"
#include <iostream>
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../Entities/PlayerBall.h"
#include "PlayerBallScript.h"
#include "../../Engine/Framework/GameObject.h"
#include "../../src/Engine/Audio/AudioSystem.h"

void CheckPointScript::AddPosition()
{
	position.push_back({ -10.0f, 1.95f, 45.7f });
	position.push_back({ 21.9f,-0.45f,108.0f });
	position.push_back({ 58.5f,-1.675f,131.69f });
	position.push_back({ 132.873f,-3.22176f,139.996 });
}

void CheckPointScript::Start()
{
	Component::Start();

	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();

	AddPosition();

	transformComponent->SetPosition(position[currentPositionInVector]);
	//添加检查点
	physx::PxMat44 physMatrix;
	memcpy(&physMatrix, &transformComponent->GetMatrix(), sizeof(physx::PxMat44));
	PxTransform pose(physMatrix);

	rigidbodyComponent->SetTrigger(PxBoxGeometry(1.0f, 0.1f, 1.0f), pose);
	//rigidbodyComponent->AddToScene();
}

void CheckPointScript::MoveCheckPoint()
{
	//检查点被触发后移动到下一个位置
	if (currentPositionInVector < position.size() - 1)
	{
		//声音
		AudioSystem::Play(AudioName::checkpoint, false);
		transformComponent->SetPosition(position[++currentPositionInVector]);
	}
	else {
		//通关
		//std::cout << "course clear!" << std::endl;
		AudioSystem::Play(AudioName::terminal, false);
		GetScene()->GetGameObject<PlayerBall>(std::string("PlayerBall"))->GetComponent<PlayerBallScript>()->SetTargetPosition(position[currentPositionInVector]);
		GetScene()->GetGameObject<PlayerBall>(std::string("PlayerBall"))->GetComponent<PlayerBallScript>()->Victory();
	}
}

void CheckPointScript::Restart()
{
	transformComponent->SetPosition(position[0]);
	currentPositionInVector = 0;
}





