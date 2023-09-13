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
    // 编辑器细节信息
    virtual void ShowDetailUI() override;
    virtual void Update(float DeltaTime) override;
    void setAggressive();
private:
    // 随机取活动范围内一点设置 mTargetPosition
    void SetNextRandomPosition();

private:
    bool bAggressive = false;                           // 是否攻击玩家（攻击/和平状态）
    float mActiveRadius = 2.5f;                         // 和平状态的活动半径
    float mMovingIntervalTime = 5.0f;                   // 和平状态每次改变位置的时间间隔
    float mElapsedTime = 0;                             // 自改变位置以来的时间，>= mMovingIntervalTime 则改变位置
    PxVec3 mCenterPosition = { -36.0f,4.0f, 2.0f };     // 活动范围的中心
    PxVec3 mTargetPosition = { -36.0f,4.0f, 2.0f };     // 和平状态目标位置
    PxVec3 mCurrentPosition;                            // 当前位置
    PxReal mPeaceForce = 60.0f;
    PxReal mAggressiveForce = 100.0f;

    // 随机数
    std::random_device mRandomDevice;
    std::mt19937 mGen;
    std::uniform_real_distribution<float> mRadiusDis;
    std::uniform_real_distribution<float> mAngleDis;

    // Components
    TransformComponent* transformComponent;
    MeshRendererComponent* meshRendererComponent;
    RigidbodyComponent* rigidbodyComponent;
    PlayerBall* mPlayerBall;

    // 物理参数
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



