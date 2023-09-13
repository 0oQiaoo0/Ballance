#pragma once

#include "GameObject.h"

#include "../../src/Engine/UI/imgui/imgui.h"

using namespace DirectX;
using namespace physx;
class Scene;
class Component
{
    friend class GameObject;
private:
    void PreTick(float DeltaTime);
    void Tick(float DeltaTime);
    void PostTick(float DeltaTime);
protected:
    GameObject* gameObject;

    virtual void Start() {}
    virtual void PreUpdate(float DeltaTime) {}
    virtual void Update(float DeltaTime) {}
    virtual void PostUpdate(float DeltaTime) {}
public:
    Scene* GetScene();

    virtual void ShowDetailUI() {};

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject(std::string name);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject(std::string name,std::string tag);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetRootGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetRootGameObject(std::string name);

    template<class T>
    bool DeleteRootGameObject();


    bool DeleteRootGameObject(std::string name);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject(std::string name);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject(std::string name, std::string tag);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetSubGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetSubGameObject(std::string name);

    template<class T>
    bool DeleteSubGameObject();


    bool DeleteSubGameObject(std::string name);

    template<class T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T*> AddComponent();

    template<class T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T*> GetComponent();

    template<class T>
    bool DeleteComponent();
};

template <class T>
std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddRootGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->AddRootGameObject<T>();
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddRootGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->AddRootGameObject<T>(name);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddRootGameObject(std::string name, std::string tag)
{
    if (!gameObject) return nullptr;

    return gameObject->AddRootGameObject<T>(name,tag);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::GetRootGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->GetRootGameObject<T>();
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::GetRootGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->GetRootGameObject<T>(name);
}

template<class T>
inline bool Component::DeleteRootGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->DeleteRootGameObject<T>();
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddSubGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->AddSubGameObject<T>();
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddSubGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->AddSubGameObject<T>(name);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::AddSubGameObject(std::string name, std::string tag)
{
    if (!gameObject) return nullptr;

    return gameObject->AddSubGameObject<T>(name, tag);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::GetSubGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->GetSubGameObject<T>();
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> Component::GetSubGameObject(std::string name)
{
    if (!gameObject) return nullptr;

    return gameObject->GetSubGameObject<T>(name);
}

template<class T>
inline bool Component::DeleteSubGameObject()
{
    if (!gameObject) return nullptr;

    return gameObject->DeleteSubGameObject<T>();
}

template <class T>
std::enable_if_t<std::is_base_of_v<Component, T>, T*> Component::AddComponent()
{
    if (!gameObject) return nullptr;

    return gameObject->AddComponent<T>();
}

template <class T>
std::enable_if_t<std::is_base_of_v<Component, T>, T*> Component::GetComponent()
{
    if (!gameObject) return nullptr;

    return gameObject->GetComponent<T>();
}

template<class T>
inline bool Component::DeleteComponent()
{
    if (!gameObject) return nullptr;

    return gameObject->DeleteComponent<T>();
}
