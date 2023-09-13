#pragma once
#include "../../Engine/Framework/GameObject.h"
#include "../../Engine/Framework/NetGameObject.h"

class PlayerBall : public NetGameObject
{
public:
    void Start() override;
    // 编辑器细节信息
    virtual void ShowDetailUI() override;
    // 各种和游戏有关的信息
    void ShowGameUI();
};



