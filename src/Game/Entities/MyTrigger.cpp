#include "MyTrigger.h"

#include "../Scripts/MyTriggerScript.h"

void MyTrigger::Start()
{
	GameObject::Start();
	AddComponent<MyTriggerScript>();
}
