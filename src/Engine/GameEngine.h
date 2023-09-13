#pragma once
#include <memory>

#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Render/Renderer.h"
#include "UI/UISystem.h"
#include "Audio/AudioSystem.h"
#include "../Game/Timer.h"

class GameEngine
{
private:
    // 目标帧率
    const int targetFPS = 100;
    // 计算每帧的时间间隔（以毫秒为单位）
    const int frameTime = 1000 / targetFPS;
public:
    void Tick(float DeltaTime);
    void Init();

    std::unique_ptr<Scene> mScene;

    std::shared_ptr<UISystem> mUISystem;

    std::shared_ptr<Renderer> mRenderer;

    std::shared_ptr<InputSystem> mInputSystem;

    std::shared_ptr<AudioSystem> mAudioSystem;

    void Realese() {};
    ~GameEngine();

    template<class T>
    void LoadScene();
};

template<class T>
void GameEngine::LoadScene()
{
    mScene = std::make_unique<T>();

    mScene->renderScene->BindRenderer(mRenderer);
    mScene->Start();

    mUISystem->BindScene(mScene.get());
}