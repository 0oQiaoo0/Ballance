#include "Component.h"

#include "../core/macro.h"

void Component::PreTick(float DeltaTime)
{
    PreUpdate(DeltaTime);
}

void Component::Tick(float DeltaTime)
{
    Update(DeltaTime);
}

void Component::PostTick(float DeltaTime)
{
    PostUpdate(DeltaTime);
}

Scene* Component::GetScene()
{
    if (!gameObject)
    {
        LOG_ERROR("can not find Scene");
        return nullptr;
    }
    return gameObject->GetScene();
}

bool Component::DeleteRootGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->DeleteRootGameObject(name);
}

bool Component::DeleteSubGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->DeleteSubGameObject(name);
}