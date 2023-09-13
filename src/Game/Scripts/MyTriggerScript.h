#pragma once
#include <chrono>

#include "../../Engine/Framework/Component.h"
#include "../Utils.h"

struct TriggerPair
{
    GameObject* other;
    GameObject* trigger;
};

class MyTriggerScript :public Component, public PxSimulationEventCallback
{
public:
    std::chrono::steady_clock::time_point lastTriggerTime; // 上次触发处理的时间点
    std::chrono::milliseconds triggerInterval{ 100 };//trigger触发间隔为100ms

    virtual void	onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
    virtual void	onTrigger(PxTriggerPair* pairs, PxU32 count);
    virtual void	onConstraintBreak(PxConstraintInfo*, PxU32) {}
    virtual void	onWake(PxActor**, PxU32) {}
    virtual void	onSleep(PxActor**, PxU32) {}
    virtual void	onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
    virtual void    ShowDetailUI() override;

    void Start() override;
    void Update(float DeltaTime) override;

private:
    bool isTriggerCheckPoint = false;
    bool isTriggerLifeProp = false;
    bool isTriggerTimeProp = false;
    bool isTriggerWoodTrans = false;
    bool isTriggerStoneTrans = false;
    bool isTriggerPaperTrans = false;
    bool isTriggerRobotBall = false;
    TriggerPair triggerPair;
    void TriggerCheckPoint();
    void TriggerLifeProp();
    void TriggerTimeProp();
    void TriggerTransformer(Utils::BallType transformerType);
    void TriggerRobotBall();
};