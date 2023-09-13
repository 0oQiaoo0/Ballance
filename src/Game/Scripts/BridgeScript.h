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

    PxJointLinearLimit linearLimit = { 0.025f, PxSpring(90.0f, 5.0f) };//ľ�������ƶ�����,����:�ƶ�����,(����ǿ��,����)
    PxJointAngularLimitPair twistLimit={-PxPi / 15, PxPi / 15};//ľ����x����ת�Ƕ�����,����:��ת�Ƕȵ�������
    /*****************************Default Settings*****************************/
public:
    void Start() override;
    void CreateBridgeDefault(PxTransform transform);
    void CreateBridge(PxTransform transform,int plankNum);
};