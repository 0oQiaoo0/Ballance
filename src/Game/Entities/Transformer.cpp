#include "Transformer.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../Scripts/TransformerScript.h"

void Transformer::Start()
{
	GameObject::Start();

	AddComponent<RigidbodyComponent>();

	AddComponent<TransformerScript>();
}
