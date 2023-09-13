#pragma once
#include "../../Engine/Framework/NetGameObject.h"

class Obstacle : public NetGameObject
{
public:
    void Start() override;
    // ±à¼­Æ÷Ï¸½ÚÐÅÏ¢
    virtual void ShowDetailUI() override;
};