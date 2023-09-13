#include "Model.h"

#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../Scripts/ModelScript.h"

void Model::Start()
{
    GameObject::Start();
    AddComponent<MeshRendererComponent>();
    AddComponent<ModelScript>();
}

void Model::ShowDetailUI()
{
    return;
}
