#pragma once
#include "../../Engine/Framework/NetGameObject.h"

class Obstacle : public NetGameObject
{
public:
    void Start() override;
    // �༭��ϸ����Ϣ
    virtual void ShowDetailUI() override;
};