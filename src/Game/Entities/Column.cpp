#include "Column.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../Scripts/ColumnScript.h"

void Column::Start()
{
	GameObject::Start();
	AddComponent<RigidbodyComponent>();
	AddComponent<MeshRendererComponent>();
	AddComponent<ColumnScript>();
}
