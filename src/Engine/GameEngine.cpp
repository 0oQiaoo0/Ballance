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
    // ��¼��ǰʱ���
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
    // �����Ѿ����ĵ�ʱ��
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
    // ���ִ��ʱ��С��֡ʱ�䣬��˯��ʣ��ʱ��
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

    //���س���
    LoadScene<TestScene>();

}

GameEngine::~GameEngine()
{
    LOG_INFO("GameEngine destory");
}


