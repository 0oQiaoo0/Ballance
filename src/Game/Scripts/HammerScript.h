#pragma once
#include "../../Engine/Framework/Component.h"
using namespace std;
class TransformComponent;
class RigidbodyComponent;
class HammerScript :public Component
{
private:
    TransformComponent* transformComponent;
    PxPhysics* gPhys;
    std::vector<PxJoint*> joints;
    /*****************************Default Settings*****************************/
    PxReal driveVelocity = 3.0f;
    PxReal plankHalfHeight = 1.0f;
    PxReal sHalfLength = 0.1f;//´¸±ú°ë³¤°ë¿í
    PxReal lHalfLength = 0.4f;//´¸Í·°ë³¤°ë¿í
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void CreateHammerDefault(PxTransform transform);
    void CreateHammer(PxTransform transform, PxReal scale, PxReal velocity);
};