#include "ModelPiece.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"

void ModelPiece::Start()
{
	GameObject::Start();
	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();
}