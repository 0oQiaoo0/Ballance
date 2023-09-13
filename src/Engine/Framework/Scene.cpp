#include "Scene.h"

#include <memory>

#include "../core/GlobalContext.h"
#include "../GameEngine.h"
#include "Components/RigidbodyComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/TransformComponent.h"

Scene::Scene()
{
    renderScene = std::make_unique<RenderScene>();
    physScene = std::make_unique<PhysScene>();
}

void Scene::Start()
{
    renderScene->Initialize();
    physScene->Initialize();
}

void Scene::PreTick(float DeltaTime)
{
    PreUpdate(DeltaTime);
    if (!bPaused || !bPlaying) 
    {
        physScene->Tick(DeltaTime);
        for (auto& object : rootGameObjects)
        {
            object.second->PreTick(DeltaTime);
        }
    }
}

void Scene::Tick(float DeltaTime)
{
    Update(DeltaTime);
    if (!bPaused || !bPlaying)
    {
        for (auto& object : rootGameObjects)
        {
            object.second->Tick(DeltaTime);
        }
    }
    renderScene->Tick(DeltaTime);
}

void Scene::PostTick(float DeltaTime)
{
    PostUpdate(DeltaTime);
    if (!bPaused || !bPlaying)
    {
        for (auto& object : rootGameObjects)
        {
            object.second->PostTick(DeltaTime);
        }
    }
}

void Scene::SetRenderCamera(Camera* camera)
{
    renderScene->BindRendererCamera(camera);
}

std::unordered_map<std::string, std::unique_ptr<GameObject>>& Scene::GetAllGameObjects()
{
    return rootGameObjects;
}

DefaultRenderItemUpdateInfo Scene::AddDefaultRenderItem(DefaultShape Shape, XMFLOAT3 TexScale, XMMATRIX WorldTransform)
{
    return renderScene->AddDefaultRenderItem(Shape, WorldTransform ,TexScale);
}

bool Scene::DeleteGameObject(std::string name)
{
    if (rootGameObjects.count(name) == 0) return false;

    if (rootGameObjects[name]->GetComponent<RigidbodyComponent>())
        rootGameObjects[name]->GetComponent<RigidbodyComponent>()->RemoveFromScene();
    if (rootGameObjects[name]->GetComponent<MeshRendererComponent>())
        rootGameObjects[name]->GetComponent<MeshRendererComponent>()->PretendRemove();
    

    for (auto i = rootGameObjects[name]->subGameObjects.begin(); i != rootGameObjects[name]->subGameObjects.end();)
    {
        rootGameObjects[name]->DeleteSubGameObject(i->second->name);
        i = rootGameObjects[name]->subGameObjects.erase(i);
    }
    
    rootGameObjects.erase(name);
    return true;
}