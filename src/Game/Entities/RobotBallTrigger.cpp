#include "RobotBallTrigger.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

#include "../Scripts/RBallTriggerScript.h"

void RobotBallTrigger::Start()
{
	GameObject::Start();

	AddComponent<RigidbodyComponent>();

	AddComponent<RBallTriggerScript>();
}
