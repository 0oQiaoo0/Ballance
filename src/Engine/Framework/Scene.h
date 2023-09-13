#pragma once

#include "../Physics/PhysScene.h"
#include "../Render/RenderScene.h"

#include "GameObject.h"

class Camera;
class Scene
{
    friend class GameEngine;
    friend class GameObject;
private:
    std::unordered_map<std::string,std::unique_ptr<GameObject>> rootGameObjects;

    void PreTick(float DeltaTime);
    void Tick(float DeltaTime);
    void PostTick(float DeltaTime);
protected:
    virtual void Start();
    virtual void PreUpdate(float DeltaTime) {}
    virtual void Update(float DeltaTime) {}
    virtual void PostUpdate(float DeltaTime) {}
public:
    std::unique_ptr<RenderScene> renderScene;
    std::unique_ptr<PhysScene> physScene;

    Scene();

    virtual void Restart() {}
    virtual void InitMovableObject() {}
    /**
     * \brief 设置主要场景摄像机
     * \param camera
     */
    void SetRenderCamera(Camera* camera);

    std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetAllGameObjects();

    /**
     * \brief 生成一个自定义形状到渲染场景中，未完善
     * \param Shape 形状
     * \param TexScale 纹理缩放
     * \param WorldTransform 世界变换矩阵
     * \return
     */
   DefaultRenderItemUpdateInfo AddDefaultRenderItem(DefaultShape Shape, XMFLOAT3 TexScale, XMMATRIX WorldTransform);

    /**
    * \brief 生成一个T类型GameObject到场景中，其name为类名，重名直接返回
    * \param T GameObject的子类
    * \return 生成的T*，重名则返回nullptr
    */
    template<class T>
    T* AddGameObject();

    template<class T>
    T* AddGameObject(std::string name);

    template<class T>
    T* AddGameObject(std::string name, std::string tag);
    /*
    * \brief 按类类型查找GameObject，返回第一个查找到的该类对象！
    * \param T GameObject的类型
    * \return 查找到的T*，若未找到为nullptr
    */
    template<class T>
    T* GetGameObject();
    /*
    * \brief 按名字查找GameObject
    * \param T GameObject的类型
    * \return 查找到的T*，若未找到为nullptr
    */
    template<class T>
    T* GetGameObject(std::string name);

    template<class T>
    bool DeleteGameObject();

    bool DeleteGameObject(std::string name);
   
    // 部分游戏逻辑
public:
    bool bPlaying = false;
    bool bPaused = true;
};

template<class T>
inline T* Scene::AddGameObject()
{
    std::string name = typeid(T).name();
    
    return AddGameObject<T>(name, "");
}

template<class T>
inline T* Scene::AddGameObject(std::string name)
{
    return AddGameObject<T>(name, "");
}

template<class T>
inline T* Scene::AddGameObject(std::string name, std::string tag)
{
    if (rootGameObjects.count(name) > 0)
    {
        std::cout << "Cannot Add GameObject";
        return nullptr;
    }
    //Add GameObject to Scene
    rootGameObjects[name] = std::make_unique<T>();
    //Add Scene to GameObject
    rootGameObjects[name]->scene = this;

    rootGameObjects[name]->name = name;
    rootGameObjects[name]->tag = tag;

    //Awake
    rootGameObjects[name]->Start();

    return dynamic_cast<T*>(rootGameObjects[name].get());
}

template<class T>
inline T* Scene::GetGameObject()
{
    for (auto& i : rootGameObjects)
    {
        auto temp = dynamic_cast<T*>(i.second.get());
        if (temp)
        {
            return temp;
        }
    }
    return nullptr;
}

template<class T>
inline T* Scene::GetGameObject(std::string name)
{
    if (rootGameObjects.count(name) == 0) return nullptr;

    return dynamic_cast<T*>(rootGameObjects[name].get());
}

template<class T>
inline bool Scene::DeleteGameObject()
{
    std::string name = std::string name = typeid(T).name();

    return DeleteGameObject(name);
}