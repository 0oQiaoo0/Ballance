#include "MyTriggerScript.h"
#include <iostream>

#include "../../Engine/Framework/Scene.h"
#include "../../Engine/Framework/GameObject.h"

#include "../../Engine/Framework/Components/RigidbodyComponent.h"
#include "../../Engine/Framework/Components/TransformComponent.h"

#include "../Entities/PlayerBall.h"
#include "../Entities/CheckPoint.h"
#include "../Entities/GameProp.h"
#include "../Entities/Transformer.h"
#include "../Entities/RobotBallTrigger.h"
#include "../Entities/RobotBall.h"

#include "PlayerBallScript.h"
#include "CheckPointScript.h"
#include "TransformerScript.h"

#include "../../src/Engine/Audio/AudioSystem.h"
#include "GamePropScript.h"

void MyTriggerScript::ShowDetailUI()
{
    ImGui::SeparatorText("Camera Component");
}

void MyTriggerScript::Start()
{
    Component::Start();

    //绑定回调
    GetScene()->physScene->gScene->setSimulationEventCallback(this);
}

void MyTriggerScript::Update(float DeltaTime)
{
    if (isTriggerCheckPoint)
    {
        isTriggerCheckPoint = false;
        TriggerCheckPoint();
    }
    else if (isTriggerLifeProp)
    {
        isTriggerLifeProp = false;
        TriggerLifeProp();
    }
    else if (isTriggerTimeProp)
    {
        isTriggerTimeProp = false;
        TriggerTimeProp();
    }
    else if (isTriggerWoodTrans)
    {
        isTriggerWoodTrans = false;
        TriggerTransformer(Utils::BallType::Wood);
    }
    else if (isTriggerStoneTrans)
    {
        isTriggerStoneTrans = false;
        TriggerTransformer(Utils::BallType::Stone);
    }
    else if (isTriggerPaperTrans)
    {
        isTriggerPaperTrans = false;
        TriggerTransformer(Utils::BallType::Paper);
    }
    else if (isTriggerRobotBall)
    {
        isTriggerRobotBall = false;
        TriggerRobotBall();
    }
}

void MyTriggerScript::TriggerCheckPoint()
{
    std::cout << "checkpointscrpit trigger" << std::endl;
    triggerPair.other->GetComponent<PlayerBallScript>()->
        ChangeResetPosition(PxTransform(triggerPair.trigger->GetComponent<TransformComponent>()->GetPosition()));
    triggerPair.trigger->GetComponent<CheckPointScript>()->MoveCheckPoint();
    
}

void MyTriggerScript::TriggerLifeProp()
{
    std::cout << "lifepropscript trigger" << std::endl;
    //声音
    AudioSystem::Play(AudioName::extra_life, false);
    triggerPair.other->GetComponent<PlayerBallScript>()->GetMoreLife();
    //场景中删除该生命道具
    GetScene()->DeleteGameObject(triggerPair.trigger->name);
   
}

void MyTriggerScript::TriggerTimeProp()
{
    std::cout << "timepropscript trigger" << std::endl;
    //声音
    AudioSystem::Play(AudioName::extra_time, false);
    triggerPair.other->GetComponent<PlayerBallScript>()->GetMoreTime(triggerPair.trigger->GetComponent<GamePropScript>()->extraTime);
    //场景中删除该时间道具
    GetScene()->DeleteGameObject(triggerPair.trigger->name);
}

void MyTriggerScript::TriggerTransformer(Utils::BallType transformerType)
{
    //如果小球已经是转换器材质的,不对其做修改材质的动作
    PlayerBallScript* tempScript = triggerPair.other->GetComponent<PlayerBallScript>();
    if (tempScript->playerBallType == transformerType);
    else
    {
        AudioSystem::Play(AudioName::transform, false);
        std::cout << "transformer trigger" << std::endl;
        tempScript->SetTargetPosition(triggerPair.trigger->GetComponent<TransformComponent>()->GetWorldTransform().p);
        tempScript->playerBallType = transformerType;
    }
}

void MyTriggerScript::TriggerRobotBall()
{
    GetScene()->GetGameObject<RobotBall>("RobotBall_1")->setAggressive();
}

void MyTriggerScript::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    //std::cout << "on trigger" << std::endl;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTriggerTime);

    if (elapsed >= triggerInterval)//上次触发的时间大于最小间隔才可以再次触发
    {
        for (PxU32 i = 0; i < count; i++)
        {
            if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
                continue;

            GameObject* other(static_cast<GameObject*>(pairs[i].otherActor->userData));
            GameObject* trigger(static_cast<GameObject*>(pairs[i].triggerActor->userData));
            if (dynamic_cast<PlayerBall*>(other))
            {
                if (dynamic_cast<CheckPoint*>(trigger))
                {
                    isTriggerCheckPoint = true;
                }
                else if (dynamic_cast<GameProp*>(trigger))
                {
                    switch (trigger->GetComponent<GamePropScript>()->gamePropType)
                    {
                    case Utils::GamePropType::Life: isTriggerLifeProp = true; break;
                    case Utils::GamePropType::Time: isTriggerTimeProp = true; break;
                    default:break;
                    }
                }
                else if (dynamic_cast<Transformer*>(trigger))
                {
                    //trigger = dynamic_cast<Transformer*>(trigger);
                    switch (trigger->GetComponent<TransformerScript>()->transformerType)
                    {
                        case Utils::BallType::Wood : isTriggerWoodTrans = true; break;
                        case Utils::BallType::Paper : isTriggerPaperTrans = true; break;
                        case Utils::BallType::Stone : isTriggerStoneTrans = true; break;
                        default:break;
                    }
                }
                else if (dynamic_cast<RobotBallTrigger*>(trigger))
                {
                    isTriggerRobotBall = true;
                }
                triggerPair.other = other;
                triggerPair.trigger = trigger;
                lastTriggerTime = currentTime; // 更新上次触发处理的时间点
            }

        }
    }
}


