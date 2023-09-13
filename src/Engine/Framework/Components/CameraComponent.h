#pragma once
#include "../Component.h"

class Camera;
class CameraComponent :public Component
{
public:
    void Start() override;
    std::unique_ptr<Camera> camera;

    void ShowDetailUI() override;
};
