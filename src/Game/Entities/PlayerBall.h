#pragma once
#include "../../Engine/Framework/GameObject.h"
#include "../../Engine/Framework/NetGameObject.h"

class PlayerBall : public NetGameObject
{
public:
    void Start() override;
    // �༭��ϸ����Ϣ
    virtual void ShowDetailUI() override;
    // ���ֺ���Ϸ�йص���Ϣ
    void ShowGameUI();
};



