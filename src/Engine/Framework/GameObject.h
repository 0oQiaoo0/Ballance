#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <DirectXMath.h>
#include <PxPhysicsAPI.h>

#include "Scene.h"
#include "Component.h"

using namespace DirectX;
using namespace physx;

class GameObject
{
    friend class Scene;
    friend class Component;
private:
    Scene* scene = nullptr;

    std::unordered_map<std::string,std::unique_ptr<GameObject>> subGameObjects;

    std::unordered_map<std::string,std::unique_ptr<Component>> ownComponents;

    void PreTick(float DeltaTime);
    void Tick(float DeltaTime);
    void PostTick(float DeltaTime);
protected:
    /**
     * \brief 游戏运行时每一帧都会调用的函数
     * \param DeltaTime
     */
    virtual void PreUpdate(float DeltaTime) {}
    virtual void Update(float DeltaTime) {}
    virtual void PostUpdate(float DeltaTime) {}
public:
    std::string name;
    GameObject* parent = nullptr;
    std::string tag;
     /**
      * \brief 在添加到场景，或者加载场景的时候调用
      */
    virtual void Start();

    Scene* GetScene();

    /**
     * \brief 在 UI 界面显示细节 
     * \param DeltaTime
     */
    virtual  void ShowDetailUI();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject(std::string name);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddRootGameObject(std::string name, std::string tag);

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetRootGameObject();

    template<class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetRootGameObject(std::string name);

    template<class T>
    bool DeleteRootGameObject();


    bool DeleteRootGameObject(std::string name);

    template <class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject();

    template <class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject(std::string name);

    template <class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> AddSubGameObject(std::string name, std::string tag);
    /*
    * \brief 按类类型查找GameObject，返回第一个查找到的该类对象！
    * \param T GameObject的类型
    * \return 查找到的T*，若未找到为nullptr
    */
    template <class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetSubGameObject();
    /*
    * \brief 按名字查找GameObject
    * \param T GameObject的类型
    * \return 查找到的T*，若未找到为nullptr
    */
    template <class T>
    std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GetSubGameObject(std::string name);

    template<class T>
    bool DeleteSubGameObject();

    bool DeleteSubGameObject(std::string name);

    template <class T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T*> AddComponent();

    template <class T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T*> GetComponent();

    template<class T>
    bool DeleteComponent();
private:
    bool DeleteComponent(std::string name);
};

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddRootGameObject()
{
    std::string name = typeid(T).name();

    return AddRootGameObject<T>(name, "");
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddRootGameObject(std::string name)
{
    return AddRootGameObject<T>(name, "");
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddRootGameObject(std::string name, std::string tag)
{
    if (!scene) return nullptr;

    return scene->AddGameObject<T>(name, tag);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::GetRootGameObject()
{
    std::string name = typeid(T).name();

    return GetRootGameObject<T>(name);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::GetRootGameObject(std::string name)
{
    if (!scene) return nullptr;

    return scene->GetGameObject<T>(name);
}

template<class T>
inline bool GameObject::DeleteRootGameObject()
{
    std::string name = typeid(T).name();

    return DeleteRootGameObject(name);
}

template <class T>
std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddSubGameObject()
{
    std::string name = typeid(T).name();

    return AddSubGameObject<T>(name, "");
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddSubGameObject(std::string name)
{
    return AddSubGameObject<T>(name, "");
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::AddSubGameObject(std::string name, std::string tag)
{
    if (subGameObjects.count(name) > 0)
    {
        std::cout << "Cannot Add GameObject";
        return nullptr;
    }
    //Add GameObject to parent
    subGameObjects[name] = std::make_unique<T>();
    //Add parent to GameObject
    subGameObjects[name]->scene = this->scene;
    subGameObjects[name]->parent =this;
    subGameObjects[name]->name = name;
    subGameObjects[name]->tag = tag;

    //Awake
    subGameObjects[name]->Start();
    //transform parent
    subGameObjects[name]->GetComponent<TransformComponent>()->parent =
        this->GetComponent<TransformComponent>();
    //transform son
    this->GetComponent<TransformComponent>()->sons[name] =
        subGameObjects[name]->GetComponent<TransformComponent>();
    //world transform refresh
    this->GetComponent<TransformComponent>()->UpdateSelf();

    return dynamic_cast<T*>(subGameObjects[name].get());
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::GetSubGameObject()
{
    std::string name = typeid(T).name();
    
    return GetSubGameObject<T>(name);
}

template<class T>
inline std::enable_if_t<std::is_base_of_v<GameObject, T>, T*> GameObject::GetSubGameObject(std::string name)
{
    if (subGameObjects.count(name) == 0) return nullptr;

    return subGameObjects[name].get();
}

template<class T>
inline bool GameObject::DeleteSubGameObject()
{
    std::string name = typeid(T).name();

    return DeleteSubGameObject(name);
}

template <class T>
std::enable_if_t<std::is_base_of_v<Component, T>, T*> GameObject::AddComponent()
{
    if (ownComponents.count(name) > 0) return nullptr;

    std::string name = typeid(T).name();
    //Add Component to Gameobject
    ownComponents[name] = std::make_unique<T>();
    //Add GameObject to Component
    ownComponents[name]->gameObject = this;
    
    //Awake
    ownComponents[name]->Start();
    return dynamic_cast<T*>(ownComponents[name].get());
}

template <class T>
std::enable_if_t<std::is_base_of_v<Component, T>, T*> GameObject::GetComponent()
{
    std::string name = typeid(T).name();
    if (ownComponents.count(name) == 0) return nullptr;

    return dynamic_cast<T*>(ownComponents[name].get());
}

template<class T>
inline bool GameObject::DeleteComponent()
{
    std::string name = typeid(T).name();
    
    return DeleteComponent(name);
}