#pragma once
#include "../../Engine/Framework/Component.h"
#include <vector>

class TransformComponent;
class RigidbodyComponent;
class CheckPointScript : public Component
{
private:
    TransformComponent* transformComponent;
    RigidbodyComponent* rigidbodyComponent;
    
    //检查点可以出现的所有位置
    std::vector<PxVec3> position;
    //初始位置为数组的第一个位置
    int currentPositionInVector = 0;

    void AddPosition();
    
public:
    void Start() override;
    void MoveCheckPoint();
    void Restart();
};