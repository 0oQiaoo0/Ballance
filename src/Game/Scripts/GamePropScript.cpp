#include "GamePropScript.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/core/math.h"
void GamePropScript::Start()
{
	transformComponent = GetComponent<TransformComponent>();
	rigidbodyComponent = GetComponent<RigidbodyComponent>();
	meshRendererComponent = GetComponent<MeshRendererComponent>();
}

void GamePropScript::SetGameProp(Utils::GamePropType gamePropType, PxTransform transform, float time)
{
	PxVec3 scale = {1,1,1};
	SetGameProp(gamePropType,scale,transform,time);
}

void GamePropScript::SetGameProp(Utils::GamePropType gamePropType, PxVec3 scale, PxTransform transform, float time)
{
	this->gamePropType = gamePropType;
	this->extraTime = time; 
	transformComponent->SetMatrix(PxTransform_to_XMMATRIX(transform,scale));
	rigidbodyComponent->SetTrigger(gamePropGeometry, transformComponent->GetWorldTransform());
	
	if (true)
	{
		if (this->gamePropType == Utils::GamePropType::Life)
		{
			meshRendererComponent->RenderModel("../../src/Asset/Models/Heart.obj");
		}
		else if (this->gamePropType == Utils::GamePropType::Time)
		{
			meshRendererComponent->RenderModel("../../src/Asset/Models/glbfile.glb");
		}
	}

}
