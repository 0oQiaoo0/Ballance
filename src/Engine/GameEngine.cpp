#include "GameEngine.h"
#include<iostream>
#include <memory>

#include "../Game/Scenes/TestScene.h"

#include "core/GlobalContext.h"
#include "core/macro.h"
#include "Framework/Scene.h"
#include "Render/RenderScene.h"
#include "UI/UIMain.h"
#include "UI/UILoading.h"


void GameEngine::Tick(float DeltaTime)
{
    // 记录当前时间点
    auto frameStart = std::chrono::high_resolution_clock::now();
    //renderCallback()
    //render module should tick here
    //Snippets::show();
    mInputSystem->Tick(DeltaTime);
    mUISystem->Tick(DeltaTime);
    if (mScene != nullptr)
    {
        mScene->PreTick(DeltaTime);
        mScene->Tick(DeltaTime);
        mScene->PostTick(DeltaTime);
    }
    // 计算已经消耗的时间
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
    // 如果执行时间小于帧时间，则睡眠剩余时间
    if (frameDuration < frameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameTime - frameDuration));
    }
}

void GameEngine::Init()
{
    mUISystem = std::make_shared<UISystem>(new UILoading());
    mInputSystem = std::make_shared<InputSystem>();
    mInputSystem->Initialize();
    mRenderer = std::make_shared<Renderer>();
    mAudioSystem = std::make_shared<AudioSystem>();

    if (mUISystem.get()->Init() == false)
    {
        LOG_ERROR("Fail to Init Renderer");
    }

    if (mAudioSystem->Init() == false)
    {
        LOG_ERROR("Fail to Init AudioSystem");
    }

    //加载场景
    LoadScene<TestScene>();

}

GameEngine::~GameEngine()
{
    LOG_INFO("GameEngine destory");
}


