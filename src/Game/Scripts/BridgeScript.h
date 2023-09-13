#pragma once
#include "../../Engine/Framework/Component.h"
using namespace std;
class TransformComponent;
class RigidbodyComponent;
class BridgeScript :public Component
{
private:
    TransformComponent* transformComponent;
    PxPhysics* gPhys;
    std::vector<PxJoint*> joints;
    /*****************************Default Settings*****************************/
    //bridge
    //PxTransform bridgeTransform = PxTransform(PxVec3(-1.0f, 2.0f, 20.0f), PxQuat(PxIdentity));
    PxReal plankSpacing = 0.8f;
    int plankNum = 20;

    //joint
    PxReal jointOffset = 0.4f;
    PxReal projectionLinearTolerance = 0.5f;
    PxReal breakForce = 150.0f;
    PxReal breakTorque = 150.0f;

    PxJointLinearLimit linearLimit = { 0.025f, PxSpring(90.0f, 5.0f) };//木板线性移动限制,参数:移动距离,(刚体强度,阻尼)
    PxJointAngularLimitPair twistLimit={-PxPi / 15, PxPi / 15};//木板绕x轴旋转角度限制,参数:旋转角度的上下限
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void CreateBridgeDefault(PxTransform transform);
    void CreateBridge(PxTransform transform,int plankNum);
};