#pragma once
#include "../../Engine/Framework/GameObject.h"

class Model :public GameObject
{
public:
    void Start() override;
    void ShowDetailUI() override;
};
