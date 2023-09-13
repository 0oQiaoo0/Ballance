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
    
    //������Գ��ֵ�����λ��
    std::vector<PxVec3> position;
    //��ʼλ��Ϊ����ĵ�һ��λ��
    int currentPositionInVector = 0;

    void AddPosition();
    
public:
    void Start() override;
    void MoveCheckPoint();
    void Restart();
};