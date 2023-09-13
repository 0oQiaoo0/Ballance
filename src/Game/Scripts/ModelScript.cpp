#include "ModelScript.h"

#include "../../Engine/core/ModelLoader.h"
#include "../Entities/ModelPiece.h"

#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"

void ModelScript::Start()
{
	Component::Start();
}

void ModelScript::LoadModel(std::string modelPath, bool _isStatic)
{
    //get data from ModelLoader
    const ModelLoader::ModelData& datas = ModelLoader::GetInstance()->GetModelData(modelPath);
    const std::vector<ModelLoader::Subset>& all_subsets = ModelLoader::GetInstance()->global_subsets;
    ModelLoader::Vertex* vertices = ModelLoader::GetInstance()->global_vertices.data();
    UINT32* indices = ModelLoader::GetInstance()->global_indices.data();
    //Physic Load
    for (int i = static_cast<int>(datas.subsetStart); i < datas.subsetCount + datas.subsetStart; ++i)
    {
        UINT32 VerticesCount = all_subsets[i].VertexCount;
        ModelLoader::Vertex* VericesStart = all_subsets[i].VertexStart + datas.verticeStart + vertices;
        UINT32 FaceCount = all_subsets[i].FaceCount;
        UINT32* indicesStart = indices + all_subsets[i].FaceStart * 3 + datas.indiceStart;
        auto modelPiece = AddSubGameObject<ModelPiece>("Model_" + std::to_string(i));
        modelPiece->GetComponent<RigidbodyComponent>()->SetTriangleActor(modelPiece->GetComponent<TransformComponent>()->GetWorldTransform(),
            VerticesCount, VericesStart, FaceCount, indicesStart, _isStatic);
    }
}
