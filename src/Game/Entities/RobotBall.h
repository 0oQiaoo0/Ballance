#pragma once
#include "../../Engine/Framework/GameObject.h"
#include "../../Engine/Framework/Components/TransformComponent.h"
#include "../../Engine/Framework/Components/MeshRendererComponent.h"
#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Game/Entities/PlayerBall.h"
#include <random>
class RobotBall : public GameObject
{
public:
    void Start() override;
    // �༭��ϸ����Ϣ
    virtual void ShowDetailUI() override;
    virtual void Update(float DeltaTime) override;
    void setAggressive();
private:
    // ���ȡ���Χ��һ������ mTargetPosition
    void SetNextRandomPosition();

private:
    bool bAggressive = false;                           // �Ƿ񹥻���ң�����/��ƽ״̬��
    float mActiveRadius = 2.5f;                         // ��ƽ״̬�Ļ�뾶
    float mMovingIntervalTime = 5.0f;                   // ��ƽ״̬ÿ�θı�λ�õ�ʱ����
    float mElapsedTime = 0;                             // �Ըı�λ��������ʱ�䣬>= mMovingIntervalTime ��ı�λ��
    PxVec3 mCenterPosition = { -36.0f,4.0f, 2.0f };     // ���Χ������
    PxVec3 mTargetPosition = { -36.0f,4.0f, 2.0f };     // ��ƽ״̬Ŀ��λ��
    PxVec3 mCurrentPosition;                            // ��ǰλ��
    PxReal mPeaceForce = 60.0f;
    PxReal mAggressiveForce = 100.0f;

    // �����
    std::random_device mRandomDevice;
    std::mt19937 mGen;
    std::uniform_real_distribution<float> mRadiusDis;
    std::uniform_real_distribution<float> mAngleDis;

    // Components
    TransformComponent* transformComponent;
    MeshRendererComponent* meshRendererComponent;
    RigidbodyComponent* rigidbodyComponent;
    PlayerBall* mPlayerBall;

    // �������
    PxGeometry mGeometry = PxSphereGeometry(PxReal(0.5f));
    PxReal staticFriction = 0.6f;
    PxReal dynamicFriction = 0.6f;
    PxReal restitution = 0.2f;
    PxReal mass = 16.0f;
    PxReal linearDamping = 0.5f;
    PxReal angularDamping = 0.5f;
    PxVec3 linearVelocity = { 0.0f, 0.0f, 0.0f };
    PxVec3 angularVelocity = { 0.0f, 0.0f, 0.0f };
};



