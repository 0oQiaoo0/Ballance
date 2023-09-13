#pragma once
#include "../../Engine/Framework/Scene.h"
#include "../../Engine/Audio/AudioSystem.h"
class TestScene :public Scene
{
protected:
    void Start() override;
public:
    void Update(float DeltaTime) override;
    void Restart() override;
    void InitMovableObject() override;
    void InitGameProp();

public:
    void PlayBgm(float);
    void StopBgm();
private:
    int mBgmIndex = 0;
    int mBgmNum = 3;
    std::vector<float> mBgmDuration = { 18.f, 16.f, 10.f };
    float mBgmElapsedTime = 15.f;
    float mBgmInterval = 20.f;
};
