#include "GameObject.h"

#include "Components/TransformComponent.h"
#include "../UI/imgui/imgui.h"
#include "Components/RigidbodyComponent.h"
#include "Components/MeshRendererComponent.h"

void GameObject::Start()
{
    AddComponent<TransformComponent>();
}

Scene* GameObject::GetScene()
{
    return scene;
}

void GameObject::PreTick(float DeltaTime)
{
    PreUpdate(DeltaTime);
    for (auto& component : ownComponents)
    {
        component.second->PreTick(DeltaTime);
    }
    for (auto& subGameObject : subGameObjects)
    {
        subGameObject.second->PreTick(DeltaTime);
    }
}

void GameObject::Tick(float DeltaTime)
{
    Update(DeltaTime);
    for (auto& component : ownComponents)
    {
        component.second->Tick(DeltaTime);
    }
    for (auto& subGameObject : subGameObjects)
    {
        subGameObject.second->Tick(DeltaTime);
    }
}

void GameObject::PostTick(float DeltaTime)
{
    PostUpdate(DeltaTime);
    for (auto& component : ownComponents)
    {
        component.second->PostTick(DeltaTime);
    }
    for (auto& subGameObject : subGameObjects)
    {
        subGameObject.second->PostTick(DeltaTime);
    }
}

void GameObject::ShowDetailUI()
{
    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::Text(name.c_str());
    for (auto& component : ownComponents) {
        component.second->ShowDetailUI();
    }
}

bool GameObject::DeleteRootGameObject(std::string name)
{
    if (!scene) return nullptr;

    return scene->DeleteGameObject(name);
}

bool GameObject::DeleteSubGameObject(std::string name)
{
    if (subGameObjects.count(name) == 0) return false;

    if (subGameObjects[name]->GetComponent<RigidbodyComponent>())
        subGameObjects[name]->GetComponent<RigidbodyComponent>()->RemoveFromScene();
    if (subGameObjects[name]->GetComponent<MeshRendererComponent>())
        subGameObjects[name]->GetComponent<MeshRendererComponent>()->PretendRemove();

    for (auto i = subGameObjects[name]->subGameObjects.begin(); i != subGameObjects[name]->subGameObjects.end();)
    {
        subGameObjects[name]->DeleteSubGameObject(i->second->name);
        i = subGameObjects[name]->subGameObjects.erase(i);
    }

    GetComponent<TransformComponent>()->sons.erase(name);
    //subGameObjects.erase(name);
    return true;
}

bool GameObject::DeleteComponent(std::string name)
{
    if (ownComponents.count(name) == 0) return false;

    if (name == "class RigidbodyComponent")
        GetComponent<RigidbodyComponent>()->RemoveFromScene();
    if (name == "class MeshRendererComponent")
        GetComponent<MeshRendererComponent>()->PretendRemove();
    
    ownComponents.erase(name);
    return true;
}